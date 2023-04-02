package kirill.rybakov.tjvproject.service;

import kirill.rybakov.tjvproject.dao.ImageRepository;
import kirill.rybakov.tjvproject.domain.Image;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
public class ImageService extends CrudService<Image, Long> {
    public ImageService(ImageRepository repository) {
        super(repository);
    }

    @Override
    protected Long id(Image e) {
        return e.getId();
    }

    public Collection<Image> getImagesByCategory(String category) {
        return ((ImageRepository)repository).getImagesByCategory(category);
    }
}
