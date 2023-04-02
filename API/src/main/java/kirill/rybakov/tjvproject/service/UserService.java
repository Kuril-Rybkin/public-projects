package kirill.rybakov.tjvproject.service;

import kirill.rybakov.tjvproject.dao.UserRepository;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.domain.User;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
public class UserService extends CrudService<User, String> {
    public UserService(UserRepository repository) {
        super(repository);
    }

    @Override
    protected String id(User e) {
        return e.getUsername();
    }

    /**
     * Get all listings posted by a specific user
     * @param username Username of the user
     * @return Collection of Listings made by the user
     * @throws ErroneousInputException if username does not exist
     */
    public Collection<Listing> getUserListings(String username) {
        if (readById(username).isEmpty()) {
            throw new ErroneousInputException();
        }
        return ((UserRepository)repository).getUserListings(username);
    }
}
