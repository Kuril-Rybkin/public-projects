package kirill.rybakov.tjvproject.domain;

import javax.persistence.*;

@Entity
public class Image implements DomainEntity<Long> {
    public Image() {

    }

    public Image(Listing listing, String url) {
        this.listing = listing;
        this.url = url;
    }

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @ManyToOne
    private Listing listing;

    @Column(columnDefinition="TEXT")
    private String url;

    @Override
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Listing getListing() {
        return listing;
    }

    public void setListing(Listing listing) {
        this.listing = listing;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }
}
