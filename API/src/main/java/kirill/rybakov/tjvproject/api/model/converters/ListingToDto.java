package kirill.rybakov.tjvproject.api.model.converters;

import kirill.rybakov.tjvproject.api.model.ListingDto;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.service.ListingService;
import org.springframework.stereotype.Component;

import java.util.function.Function;
import java.util.stream.StreamSupport;

@Component
public class ListingToDto implements Function<Listing, ListingDto> {
    private final ListingService listingService;
    private final ImageToDto imageToDto;
    private final UserToDto userToDto;

    public ListingToDto(ListingService listingService, ImageToDto imageToDto, UserToDto userToDto) {
        this.listingService = listingService;
        this.imageToDto = imageToDto;
        this.userToDto = userToDto;
    }

    /**
     * Convert Listing to ListingDto
     * @param listing the function argument Listing entity
     * @return ListingDTO
     */
    @Override
    public ListingDto apply(Listing listing) {
        ListingDto ret = new ListingDto();
        ret.setId(listing.getId());
        ret.setTitle(listing.getTitle());
        ret.setDescription(listing.getDescription());
        ret.setCategory(listing.getCategory());
        ret.setPrice(listing.getPrice());
        ret.setAuthor(listing.getAuthor().getFullName());
        ret.setAuthorUsername(listing.getAuthor().getUsername());
        ret.setAuthorEmail(listing.getAuthor().getEmail());
        ret.setGallery(StreamSupport.stream(listingService.getGallery(listing.getId()).spliterator(), false).map(imageToDto).toList());
        ret.setInterested(StreamSupport.stream(listingService.viewInterestedUsers(listing.getId()).spliterator(), false).map(userToDto).toList());

        return ret;
    }
}
