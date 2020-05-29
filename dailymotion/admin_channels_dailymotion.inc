<?php

function admin_channels_dailymotion($form,&$form_state){
	$rows = array();
	$form['group_canales'] = array(
	  '#type' => 'fieldset',
	  '#title'=>'Agregar Canal',
	  '#attributes' => array('style' => array('width:150px;display:inline-block')),		  
	);

	$form['group_canales']['id_channel']=array(
		'#title'=>'ID Channel',
		'#type'=>'textfield',
		'#description'=>'ID del canal',
		'#maxlength' => 15,
		'#size' => 15,
		);

	$form['group_canales']['name_channel']=array(
		'#title'=>'Nombre del canal',
		'#type'=>'textfield',
		'#description'=>'Nombre del channel',
		'#maxlength' => 20,
		'#size' => 20,
		);

	$form['group_canales']['addchannel'] = array(
		'#type' => 'submit', 
		'#value' => t('Agregar Canal'),
		'#ajax' => array(
			'callback' => '_ajaxInsertButton',
			'wrapper' => 'tablechannels',
			'method' => 'replace',
			'effect' => 'slide',
		),
		'#attributes' => array('style' => array('display:block'))
	);

	$query = db_query("SELECT id,id_channel,name_channel,weight FROM dailymotion_channels_videos order by weight asc");
	$result = $query->fetchAll();
	$form['example_items']['#tree'] = TRUE;
	foreach ($result as $item){
		$form['example_items'][$item->id] = array(
			'id_channel' => array('#markup' => $item->id_channel,),
	    	'name_channel' => array('#markup' => $item->name_channel,),
	    	'weight' => array(
				'#type' => 'weight',
				'#title' => t('Weight'),
				'#default_value' => $item->weight,
				'#delta' => 10,
				'#title_display' => 'invisible',
	    	),
		);
	}
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array('#type' => 'submit', '#value' => 'Actualizar!');
	return $form;
}

function admin_channels_dailymotion_submit($form, &$form_state) {
	foreach ($form_state['values']['example_items'] as $id => $item) {
	  db_query("UPDATE {dailymotion_channels_videos} SET weight = :weight WHERE id = :id",array(':weight' => $item['weight'], ':id' => $id));
	}
}

function theme_admin_channels_dailymotion($variables) {
	$form = $variables['form'];
	$rows = array();
	foreach (element_children($form['example_items']) as $id){
		$form['example_items'][$id]['weight']['#attributes']['class'] = array('example-item-weight');
		$rows[] = array(
			'data' => array(
				drupal_render($form['example_items'][$id]['id_channel']),
				drupal_render($form['example_items'][$id]['name_channel']),
				drupal_render($form['example_items'][$id]['weight']),
			),
			'class' => array('draggable'),
		);
	}
	$output = theme('table', array(
		'header' => array(t('id_channel'), t('name_channel'), t('Weight')),
		'rows' => $rows,
		'attributes' => array('id' => 'example-items-table','style' => array('width:auto')),
	));
	$output .= drupal_render_children($form);
	drupal_add_tabledrag('example-items-table', 'order', 'sibling', 'example-item-weight');
	return $output;
}

function _ajaxInsertButton($form,&$form_state){
	if($form_state['values']['id_channel']!="" && $form_state['values']['name_channel']!=""){
		$search_id_exists = search_if_exists_idchannel($form_state['values']['id_channel']);
		if($search_id_exists === false){
			$sql = db_insert('dailymotion_channels_videos')
				->fields(array(
					'id_channel'=>$form_state['values']['id_channel'],
					'name_channel'=>$form_state['values']['name_channel'],
				  ))
				->execute();

			$query = db_query("SELECT id_channel, name_channel FROM dailymotion_channels_videos order by id asc");
			$result = $query->fetchAll();
			foreach ($result as $key => $value) {
		    	$rows[$key]['id_channel']=$value->id_channel;
		    	$rows[$key]['name_channel']=$value->name_channel;
		    	$rows[$key]['editar']='<input type="button" value="Delete" onclick="" />';
		    	$rows[$key]['eliminar']='<a href="#">eliminar</a>';
			}			
			$form['channels']['#rows']=$rows;
			$form['channels']['#header']=array('id_channel','name_channel','editar','eliminar');
		}	
	}
	return $form['channels'];
}

function search_if_exists_idchannel($idchannel){
	$sqlVal=db_select('dailymotion_channels_videos','channels')
		->fields('channels', array('id_channel'))
		->condition('id_channel',$idchannel)
		->execute()
		->fetchField();
	if($sqlVal === false){
		return false;
	}else{
		return true;
	}
}