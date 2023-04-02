package kirill.rybakov.tjvprojectclient.controllers;

import kirill.rybakov.tjvprojectclient.domain.ImageDto;
import kirill.rybakov.tjvprojectclient.service.ImagesService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping("/images")
public class ImagesController {
    private final ImagesService imagesService;

    public ImagesController(ImagesService imagesService) {
        this.imagesService = imagesService;
    }

    @GetMapping("/category/{category}")
    public String getImagesByCategory(Model model, @PathVariable String category) {
        model.addAttribute("cat", category);
        model.addAttribute("images", imagesService.getImagesByCategory(category));

        return "/gallery/gallery";
    }
}
