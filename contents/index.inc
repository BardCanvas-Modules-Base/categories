<?php
/**
 * Categories index
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

global $modules;
?>

<!-- Expandible Text Area -->
<script type="text/javascript" src="<?=$config->full_root_path?>/lib/jquery.exptextarea.js"></script>
<script type="text/javascript"> $(document).ready(function() { $('.expandible_textarea').expandingTextArea(); }); </script>

<!-- CodeMirror (Preloads only) -->

<script                 src="<?=$config->full_root_path?>/lib/CodeMirror-5.16.2/lib/codemirror.js"></script>
<script                 src="<?=$config->full_root_path?>/lib/CodeMirror-5.16.2/mode/xml/xml.js"></script>
<script                 src="<?=$config->full_root_path?>/lib/CodeMirror-5.16.2/mode/javascript/javascript.js"></script>
<script                 src="<?=$config->full_root_path?>/lib/CodeMirror-5.16.2/mode/css/css.js"></script>
<script                 src="<?=$config->full_root_path?>/lib/CodeMirror-5.16.2/mode/htmlmixed/htmlmixed.js"></script>
<link rel="stylesheet" href="<?=$config->full_root_path?>/lib/CodeMirror-5.16.2/lib/codemirror.css">
<style type="text/css">
    .CodeMirror {
        border:      1px solid silver;
        height:      auto;
        font-family: 'Lucida Console', 'Courier new', Courier, monospace !important; font-size: 8pt;
        width:       100%;
    }
    .CodeMirror-scroll { max-height: 200px; }
</style>

<!-- Image gallery imports -->

<style type="text/css">
    .image_selector_anchor { display: block; position: relative; width: 100%; height: 100px; overflow: hidden;
                             border: 1px solid; }
</style>

<div id="image_selector_dialog" title="<?= $modules["gallery"]->language->tinymce_strings->select_image ?>"
     style="display: none; overflow: hidden;">
    <iframe id="image_selector_target" style="width: 100%; height: 100%;" frameborder="0"
            allowtransparency="true"></iframe>
</div>

<script type="text/javascript">
    var selected_image_trigger = null;
    var return_image_size      = 'full';
    var image_dialog_title     = '<?= $modules["gallery"]->language->tinymce_strings->select_image ?>';
    var invalid_type_for_image = '';
    
    function set_image(trigger, select_image_size)
    {
        selected_image_trigger = trigger;
        
        if( typeof select_image_size != 'undefined' ) return_image_size = select_image_size;
        
        var url = $_FULL_ROOT_PATH
            + '/gallery/index.php'
            + '?embedded_mode=true'
            + '&search_type=' + 'image'
            + '&callback='    + 'parent.' + 'set_selected_gallery_image'
            + '&wasuuup='     + wasuuup();
        
        $('#image_selector_target').attr('src', url);
        $('#image_selector_dialog').dialog('open');
    }
    
    function set_selected_gallery_image(
        id_media, type, file_url, thumbnail_url, width, height, embed_width
    ) {
        var $strings = $('#post_gallery_embed_strings');
        
        if( type != 'image' )
        {
            alert( invalid_type_for_image );
            
            return;
        }
        
        $('#image_selector_dialog').dialog('close');
        
        var image_to_return = return_image_size == 'full' ? file_url : thumbnail_url;
        
        var $container = $(selected_image_trigger).closest('.image_selector_container');
        $container.find('input.src_image_value').val(image_to_return);
        $container.find('.settings_image_selector').attr('src', image_to_return);
        $container.find('.settings_image_selector').closest('a').attr('href', image_to_return);
    }
    
    function remove_image(trigger)
    {
        var $container = $(trigger).closest('.image_selector_container');
        var empty_src = $container.find('.settings_image_selector').attr('data-empty-src');
        $container.find('input.src_image_value').val('');
        $container.find('.settings_image_selector').attr('src', empty_src);
    }
    
    $(document).ready(function()
    {
        $('#image_selector_dialog').dialog({
            modal:    true,
            autoOpen: false,
            width:    $(window).width() - 20,
            height:   $(window).height() - 10
        });
    });
</script>

<!-- Local stuff -->

<script type="text/javascript">
    var category_form_post_show_callbacks = [];
    var category_form_reset_callbacks     = [];
    var category_form_fill_callbacks      = [];
    var category_form_presubmit_callbacks = [];
</script>
<script type="text/javascript" src="<?= $config->full_root_path ?>/categories/media/functions~v1.8.3-3.js"></script>

<? $current_module->load_extensions("index", "top"); ?>

<div id="main_workarea">
    
    <script type="text/javascript">
        function reset_filter()
        {
            var $form = $('#filter_form');
            $form.find('input[name="search_for"]').val('');
            $form.find('select[name="limit"] options:first').prop('selected', true);
            $form.find('input[name="offset"]').val('0');
            $form.find('input[name="order"]').val('1');
        }
        
        function paginate(value)
        {
            var $form = $('#filter_form');
            $form.find('input[name="offset"]').val(value);
            $form.submit();
        }
    </script>
    
    <? $url = "browser.php?wasuuup=" . md5(mt_rand(1, 65535)); ?>
    <div id="categories_browser" class="ajax_record_browser" data-src="<?= $url ?>">
        <div class="framed_content state_highlight" style="padding: 100px 0; text-align: center;">
            <span class="fa fa-spinner fa-pulse"></span>
            <?= $language->wait ?>
        </div>
    </div>
    
</div>

<div id="form_workarea" style="display: none;">
    
    <h1 class="clearfix for_addition">
        <button class="pull-right" onclick="hide_category_form()">
            <span class="fa fa-reply"></span>
            <?= $language->return ?>
        </button>
        
        <?= $current_module->language->form->add_title; ?>
    </h1>
    
    <h1 class="clearfix for_edition" style="display: none">
        <button class="pull-right" onclick="hide_category_form()">
            <span class="fa fa-reply"></span>
            <?= $language->return ?>
        </button>
        
        <?= $current_module->language->form->edit_title; ?>
    </h1>
    
    <? include __DIR__ . "/form.inc"; ?>
</div>

<div id="category_messages" style="display: none">
    <div class="delete_confirmation"><?= unindent($current_module->language->messages->delete_confirmation) ?></div>
</div>

<? $current_module->load_extensions("index", "bottom"); ?>
