package kirill.rybakov.tjvproject.domain;

import javax.persistence.*;
import java.util.Collection;
import java.util.HashSet;
import java.util.Objects;
import java.util.Set;

@Entity
public class Listing implements DomainEntity<Long> {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false)
    private String title;

    @Column(columnDefinition = "TEXT")
    private String description;

    private Integer price;

    private String category;

    @OneToMany(mappedBy = "listing", cascade = CascadeType.ALL)
    Set<Image> gallery = new HashSet<>();

    @ManyToOne
    private User author;

    @ManyToMany
    private Set<User> interested = new HashSet<>();

    public Listing() {

    }

    public Listing(Long id, String title, String description, Integer price, String category, User author) {
        this.id = id;
        this.title = title;
        this.description = description;
        this.price = price;
        this.category = category;
        this.author = author;
    }

    public void addInterestedUser(User user) {
        interested.add(Objects.requireNonNull(user));
    }

    @Override
    public Long getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String url) {
        this.category = url;
    }

    public User getAuthor() {
        return author;
    }

    public void setAuthor(User author) {
        this.author = author;
    }

    public Collection<User> getInterested() {
        return interested;
    }

    public void setInterested(Set<User> interested) {
        this.interested = interested;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Integer getPrice() {
        return price;
    }

    public void setPrice(Integer price) {
        this.price = price;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Listing listing = (Listing) o;
        return id.equals(listing.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }
}
