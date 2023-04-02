package kirill.rybakov.tjvproject.service;

import kirill.rybakov.tjvproject.api.model.UserDto;
import kirill.rybakov.tjvproject.dao.ListingRepository;
import kirill.rybakov.tjvproject.domain.Image;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.domain.User;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;
import java.util.Collection;

@Service
public class ListingService extends CrudService<Listing, Long> {
    public ListingService(CrudRepository<Listing, Long> repository, UserService userService) {
        super(repository);
        this.userService = userService;
    }

    private final UserService userService;

    @Override
    protected Long id(Listing e) {
        return e.getId();
    }

    /**
     * Add User interested in the Listing (ManyToMany mapping). Transactional.
     * @param id ID of the listing in which the user is interested in
     * @param user UserDTO of the user which is interested in the listing
     * @throws EntityNotFoundException if the User is not found by ID with error message 'User not found' or if the Listing is not found by ID with error message 'Listing not found'
     * @throws ErroneousInputException if the User is the author of the Listing with error message 'User is author of listing'
     */
    @Transactional
    public void addInterestedUser(Long id, UserDto user) {
        var e = userService.readById(user.getUsername())
                .orElseThrow(() -> {throw new EntityNotFoundException("User not found");});

        var listing = readById(id)
                .orElseThrow(() -> {throw new EntityNotFoundException("Listing not found");});

        if (e.equals(listing.getAuthor())) {
            throw new ErroneousInputException("User is author of listing");
        }

        listing.addInterestedUser(e);
    }

    /**
     * Get all the users interested in a particular listing
     * @param id ID of the particular listing
     * @return Collection of all UserDTO interested in the listing with ID id
     * @throws EntityNotFoundException with message 'Listing not found' if listing ID is not found
     */
    public Collection<User> viewInterestedUsers(Long id) {
        readById(id)
                .orElseThrow(() -> {throw new EntityNotFoundException("Listing not found");});

        return ((ListingRepository)repository).getInterestedUsers(id);
    }

    /**
     * Get all images associated with listing
     * @param id ID of listing
     * @return Collection of Images
     * @throws EntityNotFoundException with message 'Listing not found' If the listing ID was not found
     */
    public Collection<Image> getGallery(Long id) {
        if (readById(id).isEmpty()) {
            throw new EntityNotFoundException("Listing not found");
        }

        return ((ListingRepository)repository).getGallery(id);
    }
}
