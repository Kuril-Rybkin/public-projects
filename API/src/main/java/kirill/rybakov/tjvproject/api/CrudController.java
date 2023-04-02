package kirill.rybakov.tjvproject.api;

import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import kirill.rybakov.tjvproject.service.CrudService;
import kirill.rybakov.tjvproject.service.EntityNotFoundException;
import kirill.rybakov.tjvproject.service.ErroneousInputException;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Collection;
import java.util.function.Function;
import java.util.stream.StreamSupport;

public abstract class CrudController<Entity, Dto, ID> {
    protected CrudService<Entity, ID> service;
    protected Function<Entity, Dto> entityToDto;
    protected Function<Dto, Entity> dtoToEntity;

    public CrudController(CrudService<Entity, ID> service, Function<Entity, Dto> entityToDto, Function<Dto, Entity> dtoToEntity) {
        this.service = service;
        this.entityToDto = entityToDto;
        this.dtoToEntity = dtoToEntity;
    }

    @PostMapping
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Some values, if not provided or are equal to null, are defaulted. " +
                    "User.fullName is defaulted to 'Anonymous' and User.email is defaulted to 'No email provided'. " +
                    "Listing.description and Listing.category are defaulted to an empty string and, Listing.price is defaulted to 0."),
            @ApiResponse(responseCode = "400", description = "When submitting User Entity if username less than 3 characters, " +
                    "or when submitting Listing Entity if Author or title missing or Author does not exist, " +
                    "or when submitting Image Entity if the Associated Listing does not exist or if the image URL is null or empty."),
            @ApiResponse(responseCode = "409", description = "Entity with same ID already exists")
    })
    public Dto create(@RequestBody Dto e) {
        try {
            return entityToDto.apply(service.create(dtoToEntity.apply(e)));
        } catch (ErroneousInputException ex) {
            throw new ResponseStatusException(HttpStatus.CONFLICT, ex.getMessage());
        }
    }

    @GetMapping
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200")
    })
    public Collection<Dto> read() {
        return StreamSupport.stream(service.read().spliterator(), false).map(entityToDto).toList();
    }

    @GetMapping("/{id}")
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200"),
            @ApiResponse(responseCode = "404", description = "Entity with provided ID does not exist")
    })
    public Dto readById(@PathVariable ID id) {
        var ret = service.readById(id);

        if (ret.isEmpty()) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
        else {
            return entityToDto.apply(ret.get());
        }
    }

    @PutMapping("/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    @ApiResponses({
            @ApiResponse(responseCode = "200"),
            @ApiResponse(responseCode = "400", description = "Provided ID and ID in request body DTO do not match"),
            @ApiResponse(responseCode = "404", description = "Entity to be updated does not exist")
    })
    public void update(@PathVariable ID id, @RequestBody Dto e) {
        try {
            service.update(id, dtoToEntity.apply(e));
        } catch (EntityNotFoundException ex) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        } catch (ErroneousInputException ex) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "ID in URL does not match ID in payload");
        }
    }

    @DeleteMapping("/{id}")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    @ApiResponses({
            @ApiResponse(responseCode = "204"),
            @ApiResponse(responseCode = "404", description = "Entity does not exist")
    })
    public void delete(@PathVariable ID id) {
        try {
            service.deleteById(id);
        } catch (EntityNotFoundException e) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
    }
}
