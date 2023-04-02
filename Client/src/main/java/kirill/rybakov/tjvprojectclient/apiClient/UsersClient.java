package kirill.rybakov.tjvprojectclient.apiClient;

import kirill.rybakov.tjvprojectclient.domain.ListingDto;
import kirill.rybakov.tjvprojectclient.domain.UserDto;
import kirill.rybakov.tjvprojectclient.exceptions.EntityNotFoundException;
import kirill.rybakov.tjvprojectclient.exceptions.ErroneousInputException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

import javax.ws.rs.ClientErrorException;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.Entity;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.MediaType;
import java.util.Arrays;
import java.util.Collection;

@Component
public class UsersClient {
    private final WebTarget usersEndpoint;

    public UsersClient(@Value("${server.url}") String apiUrl) {
        var c = ClientBuilder.newClient();
        usersEndpoint = c.target(apiUrl + "/users");
    }

    public UserDto create(UserDto e) {
        try {
            return usersEndpoint.request(MediaType.APPLICATION_JSON_TYPE)
                    .post(Entity.entity(e, MediaType.APPLICATION_JSON_TYPE), UserDto.class);
        }
        catch (ClientErrorException ex) {
            //Conflict if user exists
            if (ex.getResponse().getStatus() == 409) {
                throw new ErroneousInputException("User already exists");
            }
            //Bad request if invalid username
            else if (ex.getResponse().getStatus() == 400) {
                throw new ErroneousInputException("Invalid username");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public Iterable<UserDto> read() {
        try {
            var res = usersEndpoint.request(MediaType.APPLICATION_JSON_TYPE).get(UserDto[].class);
            return Arrays.asList(res);
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public UserDto readById(String username) {
        try {
            return usersEndpoint.path("/" + username).request(MediaType.APPLICATION_JSON_TYPE).get(UserDto.class);
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("User with username does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public void update(UserDto e) {
        try {
            usersEndpoint.request(MediaType.APPLICATION_JSON_TYPE).put(Entity.entity(e, MediaType.APPLICATION_JSON_TYPE));
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 400) {
                throw new ErroneousInputException("Input invalid, usernames dont match");
            }
            else if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("User does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public void delete() {
        try {
            usersEndpoint.path("/{username}").request(MediaType.APPLICATION_JSON_TYPE).delete();
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("User does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public Collection<ListingDto> getUserListings(String username) {
        var response = usersEndpoint.path("/" + username + "/listings").request(MediaType.APPLICATION_JSON_TYPE).get();
        if (response.getStatus() == 200) {
            return Arrays.asList(response.readEntity(ListingDto[].class));
        }
        else if (response.getStatus() == 404) {
            throw new EntityNotFoundException("User not found");
        }
        else {
            throw new RuntimeException(response.getStatusInfo().getReasonPhrase());
        }
    }
}
