package kirill.rybakov.tjvprojectclient.apiClient;

import kirill.rybakov.tjvprojectclient.domain.ImageDto;
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
public class ListingsClient {
    private final WebTarget listingsEndpoint;

    public ListingsClient(@Value("${server.url}") String apiUrl) {
        var c = ClientBuilder.newClient();
        listingsEndpoint = c.target(apiUrl + "/listings");
    }

    public ListingDto create(ListingDto e) {
        try {
            return listingsEndpoint.request(MediaType.APPLICATION_JSON_TYPE)
                    .post(Entity.entity(e, MediaType.APPLICATION_JSON_TYPE), ListingDto.class);
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 400) {
                throw new ErroneousInputException("No valid title or author");
            }
            else if (ex.getResponse().getStatus() == 409) {
                throw new ErroneousInputException("ID already taken");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            //Error cannot be handled since creating a post should not throw any errors
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public Iterable<ListingDto> read() {
        try {
            return Arrays.asList(listingsEndpoint.request(MediaType.APPLICATION_JSON_TYPE).get(ListingDto[].class));
        }
        catch (ClientErrorException ex) {
            throw new RuntimeException("Unknown response code");
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public ListingDto readById(Long id) {
        try {
            return listingsEndpoint.path("/" + id).request(MediaType.APPLICATION_JSON_TYPE).get(ListingDto.class);
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("Listing does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public void update(ListingDto e, Long id) {
        try {
            listingsEndpoint.path("/" + id).request(MediaType.APPLICATION_JSON_TYPE).put(Entity.entity(e, MediaType.APPLICATION_JSON_TYPE));
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 400) {
                throw new ErroneousInputException("Listing IDs do not match");
            }
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("Listing does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public void delete(Long id) {
        try {
            listingsEndpoint.path("/" + id).request(MediaType.APPLICATION_JSON_TYPE).delete();
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("Listing does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public Collection<ImageDto> getGallery(Long id) {
        try {
            return Arrays.asList(listingsEndpoint.path("/" + id + "/gallery").request(MediaType.APPLICATION_JSON_TYPE).get(ImageDto[].class));
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("Listing does not exist");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }

    public void addInterested(Long id, UserDto user) {
        var r = listingsEndpoint.path("/" + id + "/interest").request(MediaType.APPLICATION_JSON_TYPE)
                .post(Entity.entity(user, MediaType.APPLICATION_JSON_TYPE));

        if (r.getStatus() == 204) {
            return;
        }
        else if (r.getStatus() == 400) {
            throw new ErroneousInputException("You are the author of the listing");
        }
        else if (r.getStatus() == 404) {
            throw new EntityNotFoundException("User or listing not found");
        }
        else {
            throw new RuntimeException("Unknown response code");
        }
    }

    public Collection<UserDto> getInterested(Long id) {
        try {
            return Arrays.asList(listingsEndpoint.path("/" + id + "/interested").request(MediaType.APPLICATION_JSON_TYPE).get(UserDto[].class));
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 404) {
                throw new EntityNotFoundException("Listing not found");
            }
            else {
                throw new RuntimeException("Unknown response code");
            }
        }
        catch (Exception ex) {
            throw new RuntimeException("Something went wrong, please try again later");
        }
    }
}
