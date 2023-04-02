package kirill.rybakov.tjvproject.api;

import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import kirill.rybakov.tjvproject.api.model.ImageDto;
import kirill.rybakov.tjvproject.api.model.ListingDto;
import kirill.rybakov.tjvproject.api.model.UserDto;
import kirill.rybakov.tjvproject.api.model.converters.*;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.service.EntityNotFoundException;
import kirill.rybakov.tjvproject.service.ErroneousInputException;
import kirill.rybakov.tjvproject.service.ListingService;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Collection;
import java.util.stream.StreamSupport;

@RestController
@RequestMapping("/listings")
public class ListingController extends CrudController<Listing, ListingDto, Long> {
    private final ListingService listingService;
    private final UserToDto userToDto;
    private final ImageToDto imageToDto;

    public ListingController(ListingService service,
                             ListingToDto listingToDto,
                             ListingToEntity listingToEntity,
                             ListingService listingService,
                             UserToDto userToDto,
                             ImageToDto imageToDto) {
        super(service, listingToDto, listingToEntity);
        this.listingService = listingService;
        this.userToDto = userToDto;
        this.imageToDto = imageToDto;
    }

    @PostMapping("/{id}/interest")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    @ApiResponses({
            @ApiResponse(responseCode = "200"),
            @ApiResponse(responseCode = "400", description = "If the user ID is the author of the listing"),
            @ApiResponse(responseCode = "404", description = "If listing or user do not exist")
    })
    public void addInterestedUser(@PathVariable Long id, @RequestBody UserDto user) {
        try {
            listingService.addInterestedUser(id, user);
        }
        catch (EntityNotFoundException ex) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND, ex.getMessage());
        }
        catch (ErroneousInputException ex) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, ex.getMessage());
        }
    }

    @GetMapping("/{id}/interested")
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Return a collection of Users interested in post id"),
            @ApiResponse(responseCode = "404", description = "If listing ID is not found")
    })
    public Collection<UserDto> viewInterestedUsers(@PathVariable Long id) {
        try {
            return StreamSupport.stream(listingService.viewInterestedUsers(id).spliterator(), false).map(userToDto).toList();
        }
        catch (EntityNotFoundException ex) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND, ex.getMessage());
        }
    }

    @GetMapping("/{id}/gallery")
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200"),
            @ApiResponse(responseCode = "404", description = "If listing does not exist")
    })
    public Collection<ImageDto> getListingGallery(@PathVariable Long id) {
        try {
            return StreamSupport.stream(listingService.getGallery(id).spliterator(), false).map(imageToDto).toList();
        }
        catch (EntityNotFoundException ex) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND, ex.getMessage());
        }
    }
}
