package kirill.rybakov.tjvproject.integrationTests;

import kirill.rybakov.tjvproject.api.UserController;
import kirill.rybakov.tjvproject.api.model.UserDto;
import kirill.rybakov.tjvproject.domain.User;
import kirill.rybakov.tjvproject.service.UserService;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.web.server.ResponseStatusException;

@SpringBootTest
public class UserControllerIntegrationTests {
    @Autowired
    UserController userController;

    @Autowired
    UserService userService;

    @Test
    public void testDuplicateUsernames() {
        User user = new User();
        user.setUsername("test");
        userService.create(user);

        UserDto duplicateUser = new UserDto();
        duplicateUser.setUsername("test");

        //Check that HTTP 409 is thrown
        Assertions.assertThrows(ResponseStatusException.class, () -> userController.create(duplicateUser));
    }

    @Test
    public void testDuplicateNames() {
        User user1 = new User();
        user1.setUsername("test1");
        user1.setFullName("test user");
        user1.setEmail("email1"); //create unique email to differentiate them
        userService.create(user1);

        UserDto user2 = new UserDto();
        user2.setUsername("test2");
        user2.setFullName("test user");
        user2.setEmail("email2"); //different unique email

        userController.create(user2);
        assert(userController.readById("test1").getEmail().equals("email1"));
        assert(userController.readById("test2").getEmail().equals("email2"));
    }
}
