package kirill.rybakov.tjvprojectclient.service;

import kirill.rybakov.tjvprojectclient.apiClient.ImagesClient;
import kirill.rybakov.tjvprojectclient.domain.ImageDto;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
public class ImagesService {
    private final ImagesClient imagesClient;

    public ImagesService(ImagesClient imagesClient) {
        this.imagesClient = imagesClient;
    }

    public ImageDto create(ImageDto e) {
        return imagesClient.create(e);
    }

    public void delete(Long id) {
        imagesClient.delete(id);
    }

    public Collection<ImageDto> getImagesByCategory(String category) {
        return imagesClient.getImagesByCategory(category);
    }
}