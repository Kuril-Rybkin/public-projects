package kirill.rybakov.tjvproject.service;

import org.springframework.data.repository.CrudRepository;

import java.util.Optional;

public abstract class CrudService<T, ID> {
    protected CrudRepository<T, ID> repository;

    public CrudService(CrudRepository<T, ID> repository) {
        this.repository = repository;
    }

    protected abstract ID id(T e);

    /**
     * Create record
     * @param e Entity which is inserted into the table
     * @return Entity inserted into table if successful
     * @throws ErroneousInputException if entity already exists
     */
    public T create(T e) {
        if (id(e) != null && repository.existsById(id(e))) {
            throw new ErroneousInputException();
        }
        else {
            return repository.save(e);
        }
    }

    /**
     * Returns all entities in table
     * @return Iterable<Entity> of all found entities
     */
    public Iterable<T> read() {
        return repository.findAll();
    }

    /**
     * Read records by ID
     * @param id ID of entity which you want to read
     * @return Optional<Entity> Entity if entity was found, or null if not found
     */
    public Optional<T> readById(ID id) {
        return repository.findById(id);
    }

    /**
     * Modify record in table
     * @param id ID of existing record
     * @param e New entity to update old entity with
     * @throws EntityNotFoundException if id does not exist
     * @throws ErroneousInputException if new entity and id to replace do not match
     */
    public void update(ID id, T e) {
        if (!repository.existsById(id))
            throw new EntityNotFoundException();
        if (!id(e).equals(id))
            throw new ErroneousInputException();

        repository.save(e);
    }

    /**
     * Delete record in table by ID
     * @param id ID of record to delete
     * @throws EntityNotFoundException if ID does not exist
     */
    public void deleteById(ID id) {
        if (!repository.existsById(id))
            throw new EntityNotFoundException();

        repository.deleteById(id);
    }
}
