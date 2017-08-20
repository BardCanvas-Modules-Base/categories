
var category_form_post_show_callbacks;
var category_form_reset_callbacks;
var category_form_fill_callbacks;
var category_form_presubmit_callbacks;

var browser_position = 0;

function prepare_category_addition()
{
    var $workarea = $('#form_workarea');
    $workarea.find('.for_edition').hide();
    $workarea.find('.for_addition').show();
    
    reset_category_form();
    show_category_form();
    update_category_selector();
}

function edit_category(id_category)
{
    var $workarea = $('#form_workarea');
    $workarea.find('.for_edition').show();
    $workarea.find('.for_addition').hide();
    
    var url    = $_FULL_ROOT_PATH + '/categories/scripts/get_as_json.php';
    var params = {
        'id_category': id_category,
        'wasuuup'    : wasuuup()
    };
    
    $.blockUI(blockUI_default_params);
    $.getJSON(url, params, function(data)
    {
        if( data.message != 'OK' )
        {
            $.unblockUI();
            alert(data.message);
            
            return;
        }
        
        var record = data.data;
        var $form  = $('#category_form');
        
        reset_category_form();
        fill_category_form($form, record);
        $.unblockUI();
        show_category_form();
        update_category_selector(record.parent_category, id_category);
    });
}

function copy_category(id_category)
{
    var $workarea = $('#form_workarea');
    $workarea.find('.for_edition').hide();
    $workarea.find('.for_addition').show();
    
    var url    = $_FULL_ROOT_PATH + '/categories/scripts/get_as_json.php';
    var params = {
        'id_category': id_category,
        'wasuuup'    : wasuuup()
    };
    
    $.blockUI(blockUI_default_params);
    $.getJSON(url, params, function(data)
    {
        if( data.message != 'OK' )
        {
            $.unblockUI();
            alert(data.message);
            
            return;
        }
        
        var record         = data.data;
        record.id_category = '';
        record.slug        = '';
        
        var $form  = $('#category_form');
        
        reset_category_form();
        fill_category_form($form, record);
        $.unblockUI();
        show_category_form();
        update_category_selector(record.parent_category);
    });
}

/**
 * 
 * @param {jQuery} $form
 * @param {object} record
 */
function fill_category_form($form, record)
{
    $form.find('input[name="id_category"]').val( record.id_category );
    $form.find('input[name="title"]').val( record.title );
    $form.find('input[name="slug"]').val( record.slug ).data('modified', true);
    $form.find('textarea[name="description"]').val( record.description );
    $form.find('input[name="visibility"][value="' +  record.visibility + '"]').click();
    $form.find('input[name="min_level"][value="' +  record.min_level + '"]').closest('label').click();
    
    if( typeof category_form_fill_callbacks == 'object' )
        for(var i in category_form_fill_callbacks)
            category_form_fill_callbacks[i]($form, record);
}

function update_category_selector(preselected_id, editing_record_id)
{
    if( typeof preselected_id    == 'undefined' ) preselected_id    = '';
    if( typeof editing_record_id == 'undefined' ) editing_record_id = '';
    
    var $container = $('#parent_category_selector_container');
    $container.block(blockUI_smallest_params);
    
    var url = $_FULL_ROOT_PATH + '/categories/scripts/tree_as_json.php'
            + '?wasuuup=' + wasuuup();
    $.getJSON(url, function(data)
    {
        if( data.message != 'OK' )
        {
            alert(data.message);
            $container.unblock();
            
            return;
        }
        
        var $select = $container.find('select');
        $select.find('option:not(:first)').remove();
    
        var selected;
        for( var key in data.data )
        {
            if( key == editing_record_id ) continue;
            
            selected = key == preselected_id ? 'selected' : '';
            $select.append('<option ' + selected + ' value="' + key + '">' + data.data[key] + '</option>');
        }
        
        $container.unblock();
    });
}

function delete_category(id_category)
{
    var message = $('#category_messages').find('.delete_confirmation').text();
    
    if( ! confirm(message) ) return;
    
    var url = $_FULL_ROOT_PATH + '/categories/scripts/delete.php';
    var params = {
        'id_category': id_category,
        'wasuuup':     wasuuup()
    };
    
    var $row = $('#categories_browser_table').find('tr[data-record-id="' + id_category + '"]');
    
    $row.block(blockUI_smallest_params);
    $.get(url, params, function(response)
    {
        if( response != 'OK' )
        {
            alert(response);
            $row.unblock();
            
            return;
        }
    
        $row.unblock();
        $('#refresh_category_browser').click();
    });
}

function reset_category_form()
{
    var $form = $('#category_form');
    $form[0].reset();
    $form.find('input[name="id_category"]').val('');
    $form.find('input[name="slug"]').data('modified', false);
    
    if( typeof category_form_reset_callbacks == 'object' )
        for(var i in category_form_reset_callbacks)
            category_form_reset_callbacks[i]($form);
}

function show_category_form()
{
    browser_position = $(window).scrollTop();
    
    $('#main_workarea').hide('fast');
    $('#form_workarea').show('fast', function()
    {
        if( typeof category_form_post_show_callbacks == 'object' )
            for(var i in category_form_post_show_callbacks)
                category_form_post_show_callbacks[i]();
    });
    
    $.scrollTo(0, 250);
}

function hide_category_form()
{
    $('#form_workarea').hide('fast');
    $('#main_workarea').show('fast', function() { $.scrollTo(browser_position, 250); });
}

function prepare_category_form_serialization($form, options)
{
    if( typeof category_form_presubmit_callbacks == 'object' )
        for(var i in category_form_presubmit_callbacks)
            category_form_presubmit_callbacks[i]($form);
}

function prepare_category_form_submission(formData, $form, options)
{
    $.blockUI(blockUI_default_params);
}

function process_category_form_response(response)
{
    $.unblockUI();
    if( response != 'OK' )
    {
        alert( response );
        return;
    }
    
    hide_category_form();
    $('#refresh_category_browser').click();
}

$(document).ready(function()
{
    $('#category_form').ajaxForm({
        target: '#category_form_target',
        beforeSerialize: prepare_category_form_serialization,
        beforeSubmit:    prepare_category_form_submission,
        success:         process_category_form_response
    });
});
