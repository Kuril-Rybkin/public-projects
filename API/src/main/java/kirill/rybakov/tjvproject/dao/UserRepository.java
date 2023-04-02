package kirill.rybakov.tjvproject.dao;

import kirill.rybakov.tjvproject.domain.Listing;
import kirill.rybakov.tjvproject.domain.User;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import java.util.Collection;

@Repository
public interface UserRepository extends CrudRepository<User, String> {
    @Query("SELECT u.listings FROM User u WHERE u.username = :username")
    Collection<Listing> getUserListings(String username);
}
