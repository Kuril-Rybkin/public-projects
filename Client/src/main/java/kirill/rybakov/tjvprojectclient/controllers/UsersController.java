package kirill.rybakov.tjvprojectclient.controllers;

import kirill.rybakov.tjvprojectclient.domain.UserDto;
import kirill.rybakov.tjvprojectclient.service.UsersService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("/users")
public class UsersController {
    private final UsersService usersService;

    public UsersController(UsersService usersService) {
        this.usersService = usersService;
    }

    @GetMapping
    public String getUsers(Model model) {
        model.addAttribute("users", usersService.read());
        return "user/users";
    }

    @GetMapping("/{username}")
    public String getUserProfile(Model model, @PathVariable String username) {
        try {
            var e = usersService.readById(username);
            var listings = usersService.getUserListings(username);
            model.addAttribute("user", e);
            model.addAttribute("listings", listings);
            model.addAttribute("interestedUser", new UserDto());
            return "user/user";
        }
        catch (Exception ex) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", ex.getMessage());
            return getUsers(model);
        }
    }

    @GetMapping("/create")
    public String addUser(Model model) {
        model.addAttribute("userDto", new UserDto());
        return "user/createuser";
    }

    @PostMapping("/create")
    public String addUserSubmit(@ModelAttribute UserDto data, Model model) {
        try {
            UserDto post = usersService.create(data);
            model.addAttribute("success", true);
            model.addAttribute("successMessage", "User successfully created");
            model.addAttribute("postModel", post);
        }
        catch (Exception e) {
            model.addAttribute("success", false);
            model.addAttribute("errorMessage", e.getMessage());
            model.addAttribute("postModel", data);
        }
        return "user/createuser";
    }
}
