package kirill.rybakov.tjvproject.dao;

import kirill.rybakov.tjvproject.domain.Image;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import java.util.Collection;

@Repository
public interface ImageRepository extends CrudRepository<Image, Long> {

    @Query("SELECT i FROM Image i WHERE i.listing.category = :category")
    Collection<Image> getImagesByCategory(String category);
}
