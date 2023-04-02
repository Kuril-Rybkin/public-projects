package kirill.rybakov.tjvproject.api.model;

public class ImageDto {
    private Long id;

    private Long listing;

    private String url;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Long getListing() {
        return listing;
    }

    public void setListing(Long listing) {
        this.listing = listing;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }
}
