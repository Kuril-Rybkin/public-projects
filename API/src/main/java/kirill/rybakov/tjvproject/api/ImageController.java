package kirill.rybakov.tjvproject.api;

import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import kirill.rybakov.tjvproject.api.model.ImageDto;
import kirill.rybakov.tjvproject.api.model.converters.ImageToDto;
import kirill.rybakov.tjvproject.domain.Image;
import kirill.rybakov.tjvproject.service.CrudService;
import kirill.rybakov.tjvproject.service.ImageService;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;
import java.util.function.Function;

@RestController
@RequestMapping("/images")
public class ImageController extends CrudController<Image, ImageDto, Long>{
    ImageService imageService;
    ImageToDto imageToDto;

    public ImageController(CrudService<Image, Long> service, Function<Image, ImageDto> entityToDto, Function<ImageDto, Image> dtoToEntity, ImageService imageService, ImageToDto imageToDto) {
        super(service, entityToDto, dtoToEntity);
        this.imageService = imageService;
        this.imageToDto = imageToDto;
    }

    @GetMapping("/category/{category}")
    @ResponseBody
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Return a collection of Images that fit the category based on their listing category")
    })
    public Collection<ImageDto> getImageByCategory(@PathVariable String category) {
        return imageService.getImagesByCategory(category).stream().map(imageToDto).toList();
    }
}
