package kirill.rybakov.tjvproject.domain;

import javax.persistence.*;
import java.util.Collection;
import java.util.HashSet;
import java.util.Objects;
import java.util.Set;

@Entity
@Table(name = "user_table")
public class User implements DomainEntity<String> {
    @Override
    public String getId() {
        return username;
    }

    @Id
    private String username;

    private String fullName;

    private String email;

    @OneToMany(mappedBy = "author", cascade = CascadeType.ALL)
    private Collection<Listing> listings;

    @ManyToMany(mappedBy = "interested")
    private Set<Listing> interest = new HashSet<>();

    public User() {

    }

    public User(String username, String fullName, String email) {
        this.username = username;
        this.fullName = fullName;
        this.email = email;
    }

    public void addInterestedListing(Listing listing) {
        interest.add(Objects.requireNonNull(listing));
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getFullName() {
        return fullName;
    }

    public void setFullName(String fullName) {
        this.fullName = fullName;
    }

    public Collection<Listing> getListings() {
        return listings;
    }

    public void setListings(Collection<Listing> posts) {
        this.listings = posts;
    }

    public Collection<Listing> getInterest() {
        return interest;
    }

    public void setInterest(Set<Listing> interest) {
        this.interest = interest;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        User user = (User) o;
        return username.equals(user.username);
    }

    @Override
    public int hashCode() {
        return Objects.hash(username);
    }
}
