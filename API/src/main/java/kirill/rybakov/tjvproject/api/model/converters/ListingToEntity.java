package kirill.rybakov.tjvproject.api.model.converters;

import kirill.rybakov.tjvproject.api.model.ListingDto;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.service.UserService;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;
import org.springframework.web.server.ResponseStatusException;

import java.util.function.Function;

@Component
public class ListingToEntity implements Function<ListingDto, Listing> {
    public ListingToEntity(UserService userService) {
        this.userService = userService;
    }

    UserService userService;

    /**
     * Converts ListingDto to Listing
     * @param listingDto the function argument ListingDTO to be converted
     * @return Resulting Listing entity
     * @throws ResponseStatusException BAD_REQUEST if Listing author or Listing title is missing or if Listing author does not exist
     */
    @Override
    public Listing apply(ListingDto listingDto) {
        if (listingDto.getAuthorUsername() == null || listingDto.getTitle() == null) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Required information missing");
        }
        else {
            if (listingDto.getDescription() == null) {
                listingDto.setDescription("");
            }
            if (listingDto.getPrice() == null) {
                listingDto.setPrice(0);
            }
            if (listingDto.getCategory() == null) {
                listingDto.setCategory("");
            }
            return new Listing(listingDto.getId(),
                    listingDto.getTitle(),
                    listingDto.getDescription(),
                    listingDto.getPrice(),
                    listingDto.getCategory(),
                    userService.readById(listingDto.getAuthorUsername()).orElseThrow(() -> new ResponseStatusException(HttpStatus.BAD_REQUEST, "Invalid Author username")));
        }
    }
}
