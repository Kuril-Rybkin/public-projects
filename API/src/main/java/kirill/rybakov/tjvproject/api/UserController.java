package kirill.rybakov.tjvproject.api;

import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import kirill.rybakov.tjvproject.api.model.ListingDto;
import kirill.rybakov.tjvproject.api.model.UserDto;
import kirill.rybakov.tjvproject.api.model.converters.ListingToDto;
import kirill.rybakov.tjvproject.api.model.converters.UserToDto;
import kirill.rybakov.tjvproject.api.model.converters.UserToEntity;
import kirill.rybakov.tjvproject.domain.User;
import kirill.rybakov.tjvproject.service.ErroneousInputException;
import kirill.rybakov.tjvproject.service.UserService;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;

import java.util.Collection;
import java.util.stream.StreamSupport;

@RestController
@RequestMapping("/users")
public class UserController extends CrudController<User, UserDto, String> {
    public UserController(UserService service, UserToDto userToDto, UserToEntity userToEntity, ListingToDto listingToDto) {
        super(service, userToDto, userToEntity);
        this.listingToDto = listingToDto;
    }

    private final ListingToDto listingToDto;

    @GetMapping("/{username}/listings")
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200"),
            @ApiResponse(responseCode = "404", description = "If User does not exist")
    })
    public Collection<ListingDto> getUserPosts(@PathVariable String username) {
        try {
            return StreamSupport.stream(((UserService)service).getUserListings(username).spliterator(), false).map(listingToDto).toList();
        }
        catch (ErroneousInputException ex) {
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
    }
}
