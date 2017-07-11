<?php
namespace hng2_modules\categories;

use hng2_repository\abstract_repository;

class categories_repository extends abstract_repository
{
    protected $row_class       = "hng2_modules\\categories\\category_record";
    protected $table_name      = "categories";
    protected $key_column_name = "id_category";
    
    protected $additional_select_fields = array(
        "( select slug  from categories c2 where c2.id_category = categories.parent_category ) as parent_category_slug",
        "( select title from categories c2 where c2.id_category = categories.parent_category ) as parent_category_title",
    );
    
    public function add_select_fields($fields_array)
    {
        $this->additional_select_fields = array_merge($this->additional_select_fields, $fields_array);
    }
    
    /**
     * @param      $id_or_slug
     * @param bool $forced
     *
     * @return category_record|null
     */
    public function get($id_or_slug, $forced = false)
    {
        global $object_cache, $mem_cache;
        
        if( ! $forced )
        {
            if( $object_cache->exists($this->table_name, $id_or_slug) )
                return $object_cache->get($this->table_name, $id_or_slug);
            
            $res = $mem_cache->get("{$this->table_name}:{$id_or_slug}");
            if( is_object($res) ) return $res;
        }
        
        $where = array("id_category = '$id_or_slug' or slug = '$id_or_slug'");
        $res   = $this->find($where, 1, 0, "");
        
        if( count($res) == 0 ) return null;
        
        $record = current($res);
        $object_cache->set($this->table_name, $id_or_slug, $record);
        $mem_cache->set("{$this->table_name}:{$id_or_slug}", $record, 0, 60*5);
        
        return $record;
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
        
        $res = $database->exec("
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
        
        if($res) $this->purge_caches();
        
        return $res;
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
    
    public function get_as_tree_for_select($where = array(), $order = "parent_category, title", $with_description = false)
    {
        global $account;
        
        if( empty($order) ) $order = "parent_category, title";
        
        if( ! $account->_exists )
            $where[] = "visibility = 'public'";
        else
            $where[] = "(
                          visibility = 'public' or visibility = 'users' or 
                          (visibility = 'level_based' and '{$account->level}' >= min_level) 
                        )";
        
        $records = $this->find($where, 0, 0, $order);
        if( empty($records) ) return array();
        
        $tree   = $this->build_tree($records, "", "");
        $return = $this->format_tree_for_selector($tree, "", $with_description);
        
        return $return;
    }
    
    public function get_ids_by_slug()
    {
        $records = $this->find(array(), 0, 0, "");
        $return  = array();
        
        foreach($records as $record) $return[$record->slug] = $record->id_category;
        
        return $return;
    }
    
    /**
     * Builds the options for a select out of a tree.
     * Watch out: no break spaces used with the bullet below!
     *
     * @param category_record[] $elements
     * @param string            $tree_prefix
     * @param bool              $with_description
     *
     * @return array [id:title, id:title, ...]
     */
    private function format_tree_for_selector(array $elements, $tree_prefix, $with_description = false, $indent_level = 0)
    {
        $return = array();
        
        foreach($elements as $element)
        {
            $bullet = $indent_level == 0 ? "" : " • ";
            $element_title = $tree_prefix . $bullet . $element->title;    
            if($with_description && ! empty($element->description))
                $element_title .= ": " . $element->description;
            $return[$element->id_category] = $element_title;
            if( $element->children )
            {
                $element_children = $this->format_tree_for_selector(
                    $element->children,
                    str_repeat("   ", $indent_level),
                    $with_description, $indent_level + 1
                );
                $return = array_merge($return, $element_children);
            }
        }
        
        return $return;
    }
    
    /**
     * @param array  $where
     * @param string $order
     *
     * @return array [id:path, id:path, ...]
     */
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
    private function build_tree(array $elements, $parent_id = "", $path = "")
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
     * Warning: mem_cache purging shouldn't be included here, but after calling the method!
     * 
     * @param $key
     *
     * @return int
     */
    public function delete($key)
    {
        global $object_cache;
        
        $deletions = 0;
        
        $children = $this->find(array("parent_category" => $key), 0, 0, "");
        if( count($children) == 0 )
        {
            //TODO: Inject moving of items to default category
            
            $object_cache->delete($this->table_name, $key);
            
            return parent::delete($key);
        }
        
        foreach($children as $child) $deletions += $this->delete($child->id_category);
        
        $object_cache->delete($this->table_name, $key);
        
        return $deletions;
    }
    
    /**
     * @param int    $cache_ttl
     * @param int    $cache_version
     * @param string $order
     * @param bool   $with_slug_paths
     *
     * @return category_record[]
     */
    public function get_for_listings($cache_ttl = 0, $cache_version = 0, $order = "title", $with_slug_paths = false)
    {
        global $account, $mem_cache;
        
        $where = array();
        
        if( ! $account->_exists )
        {
            $where[] = "visibility = 'public'";
            $cache_key = "{$this->table_name}:listing-public-v{$cache_version}";
        }
        else
        {
            $where[] = "(
                          visibility = 'public' or visibility = 'users' or 
                          (visibility = 'level_based' and '{$account->level}' >= min_level) 
                        )";
            $cache_key = "{$this->table_name}:listing-bylevel-v{$cache_version}:{$account->level}";
        }
        
        if( $cache_ttl > 0 )
        {
            $res = $mem_cache->get($cache_key);
            if( ! empty($res) ) return $res;
        }
        
        $records = $this->find($where, 0, 0, $order);
        
        if( $with_slug_paths )
        {
            $paths = $this->get_slug_paths($where);
            foreach($records as &$record)
                if( ! empty($paths[$record->id_category]) )
                    $record->path = $paths[$record->id_category];
        }
        
        if( $cache_ttl > 0 ) $mem_cache->set($cache_key, $records, 0, $cache_ttl);
        
        return $records;
    }
    
    public function get_id_by_slug($slug)
    {
        global $database;
        
        $res = $database->query("select id_category from {$this->table_name} where slug = '$slug'");
        if( $database->num_rows($res) == 0 ) return "";
        
        $row = $database->fetch_object($res);
        return $row->id_category;
    }
    
    public function purge_caches()
    {
        global $mem_cache;
        $mem_cache->purge_by_prefix("{$this->table_name}:");
    }
}
