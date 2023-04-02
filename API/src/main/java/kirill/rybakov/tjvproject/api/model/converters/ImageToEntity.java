package kirill.rybakov.tjvproject.api.model.converters;

import kirill.rybakov.tjvproject.api.model.ImageDto;
import kirill.rybakov.tjvproject.domain.Image;
import kirill.rybakov.tjvproject.service.ListingService;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;
import org.springframework.web.server.ResponseStatusException;

import java.util.function.Function;

@Component
public class ImageToEntity implements Function<ImageDto, Image> {
    private final ListingService listingService;

    ImageToEntity(ListingService listingService) {
        this.listingService = listingService;
    }

    /**
     * Convert Image DTO to Image Entity
     * @param imageDto the function argument Image DTO
     * @return Image DTO converted to Image Entity
     * @throws ResponseStatusException BAD_REQUEST if the listing associated with the image does not exist
     */
    @Override
    public Image apply(ImageDto imageDto) {
        if (imageDto.getUrl() == null || imageDto.getUrl().equals("")) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "No URL provided");
        }
        return new Image(listingService.readById(imageDto.getListing()).orElseThrow(() -> {
                    throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Listing does not exist");
                }),
                imageDto.getUrl());
    }
}
