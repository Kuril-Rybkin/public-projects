package kirill.rybakov.tjvprojectclient.apiClient;

import kirill.rybakov.tjvprojectclient.domain.ImageDto;
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
public class ImagesClient {
    private final WebTarget imagesEndpoint;

    public ImagesClient(@Value("${server.url}") String apiUrl) {
        var c = ClientBuilder.newClient();
        imagesEndpoint = c.target(apiUrl + "/images");
    }

    public ImageDto create(ImageDto e) {
        try {
            return imagesEndpoint.request(MediaType.APPLICATION_JSON_TYPE)
                    .post(Entity.entity(e, MediaType.APPLICATION_JSON_TYPE), ImageDto.class);
        }
        catch (ClientErrorException ex) {
            if (ex.getResponse().getStatus() == 400) {
                throw new ErroneousInputException("Image URL is null or empty");
            }
            else if (ex.getResponse().getStatus() == 409) {
                throw new ErroneousInputException("Image with same ID already exists");
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
            imagesEndpoint.path("/" + id).request(MediaType.APPLICATION_JSON_TYPE)
                    .delete();
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

    public Collection<ImageDto> getImagesByCategory(String category) {
        return Arrays.asList(imagesEndpoint.path("/category/" + category).request(MediaType.APPLICATION_JSON_TYPE).get(ImageDto[].class));
    }
}
