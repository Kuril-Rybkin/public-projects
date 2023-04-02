package kirill.rybakov.tjvproject.api.model;


import java.util.Collection;

public class ListingDto {
    private Long id;
    private String title;
    private String description;
    private Integer price;
    private String category;
    private String author;
    private String authorUsername;
    private String authorEmail;
    private Collection<ImageDto> gallery;
    private Collection<UserDto> interested;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
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

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getAuthorUsername() {
        return authorUsername;
    }

    public void setAuthorUsername(String authorUsername) {
        this.authorUsername = authorUsername;
    }

    public String getAuthorEmail() {
        return authorEmail;
    }

    public void setAuthorEmail(String authorEmail) {
        this.authorEmail = authorEmail;
    }

    public Collection<ImageDto> getGallery() {
        return gallery;
    }

    public void setGallery(Collection<ImageDto> gallery) {
        this.gallery = gallery;
    }

    public Collection<UserDto> getInterested() {
        return interested;
    }

    public void setInterested(Collection<UserDto> interested) {
        this.interested = interested;
    }
}
