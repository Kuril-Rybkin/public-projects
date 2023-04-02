package kirill.rybakov.tjvprojectclient.service;

import kirill.rybakov.tjvprojectclient.apiClient.ListingsClient;
import kirill.rybakov.tjvprojectclient.domain.ImageDto;
import kirill.rybakov.tjvprojectclient.domain.ListingDto;
import kirill.rybakov.tjvprojectclient.domain.UserDto;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class ListingsService {
    private final ListingsClient listingsClient;

    public ListingsService(ListingsClient ListingsClient) {
        this.listingsClient = ListingsClient;
    }

    public ListingDto create(ListingDto e) {
        return listingsClient.create(e);
    }

    public Iterable<ListingDto> read() {
        var list = listingsClient.read();
        for (var listing : list) {
            listing.setInterestedPreview(getInterested(listing.getId()));
        }
        return list;
    }

    public ListingDto readById(Long id) {
        return listingsClient.readById(id);
    }

    public void update(ListingDto e, Long id) {
        listingsClient.update(e, id);
    }

    public void delete(Long id) {
        listingsClient.delete(id);
    }

    public Iterable<ImageDto> getGallery(Long id) {
        return listingsClient.getGallery(id);
    }

    public void addInterested(Long id, UserDto user) {
        listingsClient.addInterested(id, user);
    }

    public String getInterested(Long id) {
        var list = (List<UserDto>)listingsClient.getInterested(id);

        return switch (list.size()) {
            case 0 -> "Be the first to be interested in this listing";
            case 1 -> list.get(0).getFullName() + " is interested in this listing";
            case 2 -> list.get(0).getFullName() + " and " + list.get(1).getFullName() + " are interested in this listing";
            default ->
                    list.get(0).getFullName() + ", " + list.get(1).getFullName() + " and " + (list.size() - 2) + " others are interested in this listing";
        };
    }
}
