
function reset_filter()
{
    var $form = $('#filter_form');
    $form.find('input[name="search_for"]').val('');
    $form.find('select[name="limit"] options:first').prop('selected', true);
    $form.find('input[name="offset"]').val('0');
    $form.find('input[name="order"]').val('3');
}

//noinspection JSUnusedGlobalSymbols
function paginate(value)
{
    var $form = $('#filter_form');
    $form.find('input[name="offset"]').val(value);
    $form.submit();
}

function prepare_category_addition()
{
    $('#form_workarea')
        .find('.for_edition').hide()
        .find('.for_addition').show()
    ;
    reset_category_form();
    show_category_form();
    update_category_selector();
}

function edit_category(id_category)
{
    var url    = $_FULL_ROOT_PATH + '/categories/scripts/get_as_json.php';
    var params = {
        'id_category': id_category,
        'wasuuup'    : parseInt(Math.random() * 1000000000000000)
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
        update_category_selector(record.parent_category);
    });
}

function copy_category(id_category)
{
    var url    = $_FULL_ROOT_PATH + '/categories/scripts/get_as_json.php';
    var params = {
        'id_category': id_category,
        'wasuuup'    : parseInt(Math.random() * 1000000000000000)
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
}

function update_category_selector(preselected_id)
{
    if( typeof preselected_id == 'undefined' ) preselected_id = '';
    
    var $container = $('#parent_category_selector_container');
    $container.block(blockUI_smallest_params);
    
    var url = $_FULL_ROOT_PATH + '/categories/scripts/tree_as_json.php?wasuuup=' + parseInt(Math.random() * 1000000000000000);
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
        'wasuuup':     parseInt(Math.random() * 1000000000000000)
    };
    
    $.blockUI(blockUI_smallest_params);
    $.get(url, params, function(response)
    {
        if( response != 'OK' )
        {
            alert(response);
            $.unblockUI();
            
            return;
        }
    
        $.unblockUI();
        $('#refresh_category_browser').click();
    });
}

function reset_category_form()
{
    var $form = $('#category_form');
    $form[0].reset();
    $form.find('input[name="slug"]').data('modified', false);
}

function show_category_form()
{
    $('#main_workarea').hide('fast');
    $('#form_workarea').show('fast');
}

function hide_category_form()
{
    $('#form_workarea').hide('fast');
    $('#main_workarea').show('fast');
}

function update_slug()
{
    var $form = $('#category_form');
    if( $form.find('input[name="id_category"]').val() != '' ) return;
    if( $form.find('input[name="slug"]').data('modified') ) return;
    
    var title = $form.find('input[name="title"]');
    var slug  = title.toLowerCase();
    
    slug = slug.replace(/[^a-z0-9\-_]/g, "-");
    slug = slug.replace(/\-+/g, "-");
    slug = slug.replace(/_+/g, "_");
    
    $form.find('input[name="slug"]').val(slug);
}

function prepare_category_form_submission()
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
        beforeSubmit: prepare_category_form_submission,
        success:      process_category_form_response
    });
});
