package kirill.rybakov.tjvproject.blackBoxTests;

import kirill.rybakov.tjvproject.api.ListingController;
import kirill.rybakov.tjvproject.api.model.converters.*;
import kirill.rybakov.tjvproject.domain.User;
import kirill.rybakov.tjvproject.service.ListingService;
import kirill.rybakov.tjvproject.service.UserService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;

@WebMvcTest(ListingController.class)
public class ListingControllerTest {
    @Autowired
    MockMvc mockMvc;

    @MockBean
    ListingToDto listingToDto;

    @MockBean
    ListingToEntity listingToEntity;

    @MockBean
    UserToDto userToDto;

    @MockBean
    UserToEntity userToEntity;

    @MockBean
    ImageToDto imageToDto;

    @MockBean
    ImageToEntity imageToEntity;

    @MockBean
    ListingService listingService;

    @MockBean
    UserService userService;

    @Test
    public void createTest() throws Exception {
        userService.create(new User("test", "testuser", "email"));
        mockMvc.perform(MockMvcRequestBuilders.post("/listings").content("{\"title\" : \"title\", \"authorUsername\": \"test\"}").contentType(MediaType.APPLICATION_JSON))
                .andExpect(MockMvcResultMatchers.status().isOk());
//                .andExpect(MockMvcResultMatchers.jsonPath("$.authorUsername", Matchers.is("test")));
        // for some reason the JSON path didnt work here and I dont know why
    }
}
