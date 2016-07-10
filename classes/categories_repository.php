<?php
namespace hng2_modules\categories;

use hng2_base\repository\abstract_repository;

class categories_repository extends abstract_repository
{
    protected $row_class       = "hng2_modules\\categories\\category_record";
    protected $table_name      = "categories";
    protected $key_column_name = "id_category";
    
    protected $additional_select_fields = array(
        "( select slug  from categories c2 where c2.id_category = categories.parent_category ) as parent_category_slug",
        "( select title from categories c2 where c2.id_category = categories.parent_category ) as parent_category_title",
    );
    
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
    public function save($record)
    {
        global $database;
        
        $this->validate_record($record);
        $obj = $record->get_for_database_insertion();
        
        return $database->exec("
            insert into {$this->table_name}
            (
                id_category     ,
                parent_category ,
                slug            ,
                title           ,
                description     ,
                visibility      ,
                min_level       
            ) values (
                '{$obj->id_category}',
                '{$obj->parent_category}',
                '{$obj->slug}',
                '{$obj->title}',
                '{$obj->description}',
                '{$obj->visibility}',
                '{$obj->min_level}'
            ) on duplicate key update
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
     * @throws \Exception
     */
    public function validate_record($record)
    {
        if( ! $record instanceof category_record )
            throw new \Exception(
                "Invalid object class! Expected: {$this->row_class}, received: " . get_class($record)
            );
    }
    
    public function get_as_tree_for_select($where = array(), $order = "title asc")
    {
        $records = $this->find($where, 0, 0, $order);
        if( empty($records) ) return array();
        
        $tree   = $this->build_tree($records, "", "");
        $return = $this->format_tree_for_selector($tree, "");
        
        return $return;
    }
    
    /**
     * Builds the options for a select out of a tree.
     * 
     * @param category_record[] $elements
     * 
     * @return array [id:title, id:title, ...]
     */
    private function format_tree_for_selector(array $elements, $tree_prefix)
    {
        $return = array();
        
        foreach($elements as $element)
        {
            $return[$element->id_category] = $tree_prefix . "• " . $element->title;
            if( $element->children )
            {
                # Watch out: no break spaces used with the bullet below!
                $element_children = $this->format_tree_for_selector($element->children, $tree_prefix . "  ");
                $return = array_merge($return, $element_children);
            }
        }
        
        return $return;
    }
    
    public function get_slug_paths($where = array(), $order = "title asc")
    {
        $records = $this->find($where, 0, 0, $order);
        if( empty($records) ) return array();
        
        $tree   = $this->build_tree($records, "", "");
        $return = $this->format_tree_for_paths($tree);
        
        return $return;
    }
    
    /**
     * Builds all the slug paths by reference
     * 
     * @param category_record[] $elements
     * 
     * @return array [id:path, id:path, ...]
     */
    private function format_tree_for_paths(array $elements)
    {
        $return = array();
        
        foreach($elements as $key => $element)
        {
            $return[$element->id_category] = $key;
            if( $element->children )
            {
                # Watch out: no break spaces used with the bullet below!
                $element_children = $this->format_tree_for_paths($element->children);
                $return = array_merge($return, $element_children);
            }
        }
        
        return $return;
    }
    
    /**
     * @param category_record[] $elements
     * @param string            $parent_id
     * @param                   $path
     *
     * @return array
     */
    private function build_tree(array $elements, $parent_id = "", $path)
    {
        $branch = array();
        
        foreach( $elements as $element )
        {
            if( $element->parent_category == $parent_id )
            {
                $children = $this->build_tree($elements, $element->id_category, "{$path}/{$element->slug}");
                if( $children ) $element->children = $children;
                $branch["{$path}/{$element->slug}"] = $element;
            }
        }
        
        return $branch;
    }
    
    /**
     * @param $key
     *
     * @return int
     */
    public function delete($key)
    {
        $deletions = 0;
        
        $children = $this->find(array("parent_category" => $key), 0, 0, "");
        if( count($children) == 0 )
        {
            //TODO: Inject moving of items to default category
            
            return parent::delete($key);
        }
        
        foreach($children as $child) $deletions += $this->delete($child->id_category);
        
        return $deletions;
    }
}
