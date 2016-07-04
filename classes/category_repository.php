<?php
namespace hng2_modules\categories;

use hng2_base\repository\abstract_repository;

class category_repository extends abstract_repository
{
    protected $row_class       = "hng2_modules\\categories\\category_record\\category_record";
    protected $table_name      = "categories";
    protected $key_column_name = "id_category";
    
    /**
     * @param $id
     *
     * @return category_record|null
     */
    public function get($id)
    {
        return parent::get($id);
    }
    
    /**
     * @param array  $where
     * @param int    $limit
     * @param int    $offset
     * @param string $order
     *
     * @return category_record[]
     */
    public function find($where, $limit, $offset, $order)
    {
        return parent::find($where, $limit, $offset, $order);
    }
    
    /**
     * @param category_record $record
     *
     * @return int
     */
    public function insert($record)
    {
        global $database;
        
        $this->validate_record($record);
        $obj = $record->get_for_database_insertion();
        
        return $database->exec("
            insert into {$this->table_name} set
                id_category     = '{$obj->id_category}',
                parent_category = '{$obj->parent_category}',
                slug            = '{$obj->slug}',
                title           = '{$obj->title}',
                description     = '{$obj->description}',
                visibility      = '{$obj->visibility}',
                min_level       = '{$obj->min_level}'
        ");
    }
    
    /**
     * @param category_record $record
     *
     * @return int
     */
    public function save($record)
    {
        global $database;
        
        $this->validate_record($record);
        $obj = $record->get_for_database_insertion();
        
        return $database->exec("
            update {$this->table_name} set
                parent_category = '{$obj->parent_category}',
                slug            = '{$obj->slug}',
                title           = '{$obj->title}',
                description     = '{$obj->description}',
                visibility      = '{$obj->visibility}',
                min_level       = '{$obj->min_level}'
            where
                id_category     = '{$obj->id_category}'
        ");
    }
    
    /**
     * @param category_record $record
     *
     * @throws \Exception
     */
    public function validate_record($record)
    {
        if( ! $record instanceof category_record )
            throw new \Exception(
                "Invalid object class! Expected: {$this->row_class}, received: " . get_class($record)
            );
    }
}
