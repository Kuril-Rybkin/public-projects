package kirill.rybakov.tjvproject.dao;

import kirill.rybakov.tjvproject.domain.Image;
import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.domain.User;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.Collection;

@Repository
public interface ListingRepository extends CrudRepository<Listing, Long> {
    @Query("SELECT l.gallery FROM Listing l WHERE l.id = :id")
    Collection<Image> getGallery(@Param("id") Long id);

    @Query("SELECT l.interested FROM Listing l WHERE l.id = :id")
    Collection<User> getInterestedUsers(@Param("id") Long id);
}
