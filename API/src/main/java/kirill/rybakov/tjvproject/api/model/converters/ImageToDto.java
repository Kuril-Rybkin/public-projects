package kirill.rybakov.tjvproject.api.model.converters;

import kirill.rybakov.tjvproject.api.model.ImageDto;
import kirill.rybakov.tjvproject.domain.Image;
import org.springframework.stereotype.Component;

import java.util.function.Function;

@Component
public class ImageToDto implements Function<Image, ImageDto> {
    /**
     * Converts Image entity to Image DTO
     * @param image the function argument Image entity
     * @return The Image DTO
     */
    @Override
    public ImageDto apply(Image image) {
        var ret = new ImageDto();
        ret.setId(image.getId());
        ret.setListing(image.getListing().getId());
        ret.setUrl(image.getUrl());

        return ret;
    }
}
