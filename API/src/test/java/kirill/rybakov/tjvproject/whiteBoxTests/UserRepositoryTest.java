package kirill.rybakov.tjvproject.whiteBoxTests;

import kirill.rybakov.tjvproject.dao.ImageRepository;
import kirill.rybakov.tjvproject.dao.ListingRepository;
import kirill.rybakov.tjvproject.dao.UserRepository;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.domain.User;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;

import java.util.Arrays;

@DataJpaTest
public class UserRepositoryTest {
    @Autowired
    ListingRepository listingRepository;

    @Autowired
    UserRepository userRepository;

    @Autowired
    ImageRepository imageRepository;

    @AfterEach
    void delete() {
        userRepository.deleteAll();
    }

    @Test
    public void listingsByUserTest() {
        User user1 = new User("test1", "test user 1", "email1");
        User user2 = new User("test2", "test user 2", "email2");

        userRepository.save(user1);
        userRepository.save(user2);

        Listing listing1 = new Listing();
        listing1.setAuthor(user1);
        listing1.setTitle("listing 1 by user 1");

        Listing listing2 = new Listing();
        listing2.setAuthor(user1);
        listing2.setTitle("listing 2 by user 1");

        Listing listing3 = new Listing();
        listing3.setAuthor(user2);
        listing3.setTitle("listing 3 by user 2");

        listingRepository.save(listing1);
        listingRepository.save(listing2);
        listingRepository.save(listing3);


        Assertions.assertEquals(Arrays.asList(listing1, listing2), userRepository.getUserListings("test1"));
        Assertions.assertEquals(Arrays.asList(listing3), userRepository.getUserListings("test2"));
    }
}
