package kirill.rybakov.tjvprojectclient.controllers;

import kirill.rybakov.tjvprojectclient.domain.ImageDto;
import kirill.rybakov.tjvprojectclient.domain.ListingDto;
import kirill.rybakov.tjvprojectclient.domain.UserDto;
import kirill.rybakov.tjvprojectclient.service.ImagesService;
import kirill.rybakov.tjvprojectclient.service.ListingsService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("/listings")
public class ListingsController {
    private final ListingsService listingsService;
    private final ImagesService imagesService;

    public ListingsController(ListingsService listingsService, ImagesService imagesService) {
        this.listingsService = listingsService;
        this.imagesService = imagesService;
    }

    //Create listing
    @GetMapping("/create")
    public String addListing(Model model) {
        model.addAttribute("listing", new ListingDto());
        ((ListingDto)model.getAttribute("listing")).setCategory("No category");
        return "listing/createlisting";
    }

    @PostMapping("/create")
    public String addListingSubmit(@ModelAttribute ListingDto data, Model model) {
        try {
            ListingDto listing = listingsService.create(data);
            model.addAttribute("success", true);
            model.addAttribute("errorMessage", "Listing successfully created");
            model.addAttribute("listing", listing);
        }
        catch (Exception e) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", e.getMessage());
            model.addAttribute("listing", data);
        }
        return "listing/createlisting";
    }

    //Get all listings
    @GetMapping
    public String getListings(Model model) {
        var listings = listingsService.read();
        model.addAttribute("listings", listings);
        model.addAttribute("interestedUser", new UserDto());
        return "listing/listings";
    }

    //Get specific listing
    @GetMapping("/{id}")
    public String getListing(Model model, @PathVariable Long id) {
        try {
            var e = listingsService.readById(id);
            var images = listingsService.getGallery(id);
            model.addAttribute("listing", e);
            model.addAttribute("images", images);
            model.addAttribute("interestedUser", new UserDto());
            return "listing/listing";
        }
        catch (Exception ex) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", ex.getMessage());
            return getListings(model);
        }
    }

    //Edit specific listing
    @GetMapping("/{id}/edit")
    public String editListing(Model model, @PathVariable Long id) {
        try {
            var listing = listingsService.readById(id);
            model.addAttribute("listing", listing);
            model.addAttribute("images", listingsService.getGallery(id));
            return "listing/editlisting";
        }
        catch (Exception ex) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", ex.getMessage());
            return getListings(model);
        }
    }

    //Submit editing specific listing
    @PostMapping("/{id}/edit")
    public String editListingSubmit(Model model, @PathVariable Long id, @ModelAttribute ListingDto listing) {
        try {
            listing.setAuthorUsername(listingsService.readById(id).getAuthorUsername());
            listingsService.update(listing, id);
            model.addAttribute("success", true);
            model.addAttribute("successMessage", "Listing edited successfully");
            model.addAttribute("listing", listing);
        }
        catch (Exception e) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", e.getMessage());
            model.addAttribute("listing", listing);
        }
        return "listing/editlisting";
    }

    //Get specific listing gallery
    @GetMapping("/{id}/editgallery")
    public String editListingGallery(Model model, @PathVariable Long id) {
        ImageDto newImage = new ImageDto();
        newImage.setListing(id);

        model.addAttribute("newImage", newImage);
        model.addAttribute("images", listingsService.getGallery(id));

        return "listing/editgallery";
    }

    //Edit specific listing gallery
    @PostMapping("{id}/editgallery")
    public String editListingGallerySubmit(Model model, @PathVariable Long id, @ModelAttribute ImageDto image) {
        ImageDto newImage = new ImageDto();
        newImage.setListing(id);

        try {
            imagesService.create(image);
            model.addAttribute("success", true);
            model.addAttribute("errorMessage", "Image successfully added");
            model.addAttribute("images", listingsService.getGallery(id));
            model.addAttribute("newImage", newImage);
        }
        catch (Exception e) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", e.getMessage());
            model.addAttribute("images", listingsService.getGallery(id));
            model.addAttribute("newImage", newImage);
        }
        return "listing/editgallery";
    }

    //Delete image from gallery of specific listing
    @PostMapping("/{id}/deletegallery/{idImage}")
    public String editListingGalleryDelete(Model model, @PathVariable Long id, @PathVariable Long idImage) {
        ImageDto newImage = new ImageDto();
        newImage.setListing(id);

        try {
            imagesService.delete(idImage);
            model.addAttribute("success", true);
            model.addAttribute("successMessage", "Image successfully removed");
            model.addAttribute("images", listingsService.getGallery(id));
            model.addAttribute("newImage", newImage);
        }
        catch (Exception e) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", e.getMessage());
            model.addAttribute("images", listingsService.getGallery(id));
            model.addAttribute("newImage", newImage);
        }

        return "listing/editgallery";
    }

    //Add new interested user in listing
    @PostMapping("/{id}/interest")
    public String addInterestedUser(Model model, @PathVariable Long id, @ModelAttribute UserDto user) {
        try {
            listingsService.addInterested(id, user);
            model.addAttribute("success", true);
            model.addAttribute("errorMessage", "You are successfully interested in this listing");
        }
        catch (Exception e) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", e.getMessage());
        }

        return getListing(model, id);
    }
}
