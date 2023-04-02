package kirill.rybakov.tjvprojectclient.service;

import kirill.rybakov.tjvprojectclient.apiClient.UsersClient;
import kirill.rybakov.tjvprojectclient.domain.ListingDto;
import kirill.rybakov.tjvprojectclient.domain.UserDto;
import org.springframework.stereotype.Service;

@Service
public class UsersService {
    private final UsersClient usersClient;
    private final ListingsService listingsService;

    public UsersService(UsersClient usersClient, ListingsService listingsService) {
        this.usersClient = usersClient;
        this.listingsService = listingsService;
    }

    public UserDto create(UserDto e) {
        return usersClient.create(e);
    }

    public Iterable<UserDto> read() {
        return usersClient.read();
    }

    public UserDto readById(String username) {
        return usersClient.readById(username);
    }

    public void update(UserDto e) {
        usersClient.update(e);
    }

    public void delete() {
        usersClient.delete();
    }

    public Iterable<ListingDto> getUserListings(String username) {
        var list = usersClient.getUserListings(username);
        for (var listing : list) {
            listing.setInterestedPreview(listingsService.getInterested(listing.getId()));
        }

        return list;
    }
}
