<?php

	function show_videos_dailymotion($form,&$form_state){
		$rows=array();
		$json_url = "";
		$arraycanales=array();
		$form=array();

		$query = db_query("SELECT id,id_channel,name_channel,weight FROM dailymotion_channels_videos order by weight asc");
		$result = $query->fetchAll();
		$form['example_items']['#tree'] = TRUE;

		foreach ($result as $item){
			$form['example_items'][$item->id] = array(
				
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

		/*$form['actionsz'] = array('#type' => 'actions');
		$form['actionsz']['submit'] = array('#type' => 'submit', '#value' => 'Actualizar!');*/

		$form['action_update'] = array(
			'#type' => 'submit', 
			'#value' => t('Actualizar!'),
			'#ajax' => array(
				'callback' => '_ajaxActionsUpdate',
				'wrapper' => 'action_update',
				'method' => 'replace',
				'effect' => 'slide',
			),
		);

		$query = db_query("SELECT id_channel, name_channel FROM dailymotion_channels_videos order by id asc");
		$result = $query->fetchAll();
		foreach ($result as $key => $value){
	    	$arraycanales[$value->id_channel]=$value->name_channel;
		}

		$form['canales'] = array(
	  		'#type' => 'select',
	  		'#title' => t('Canal'),
	  		'#options' => $arraycanales,
	  		'#required' => TRUE,
	  		'#ajax' => array(
	  		      'callback' => '_ajaxCanalesSelect',
	  		      'wrapper' => 'divvideos',
	  		      'method' => 'replace',
	  		      'effect' => 'slide',
	  		    ),  		
	  	);

	  	$form['actions'] = array(
	  		'#type' => 'submit', 
	  		'#value' => t('Obtener Videos'),
	  		'#ajax' => array(
	  			'callback' => '_ajaxActionsButton',
	  			'wrapper' => 'divvideos',
	  			'method' => 'replace',
	  			'effect' => 'slide',
	  		),
	  	);

	  	if (isset($form_state['values']['canales'])){
			$json_url ="https://api.dailymotion.com/playlist/".$form_state['values']['canales']."/videos?fields=channel,embed_url,id,owner,thumbnail_180_url,title,url,created_time&limit=10";
			$json = drupal_http_request($json_url, array('context' => stream_context_create(array('ssl' => array('verify_peer' => FALSE, 'verify_peer_name' => FALSE),)),));
			$data = json_decode($json->data);
			foreach ($data->list as $key => $data){
				$rows[$key]['created_time']=strftime("%Y-%m-%d %H:%M:%S",$data->created_time);
				$rows[$key]['thumbnail_180_url']='<img src="'.$data->thumbnail_180_url.'" style="width:100px;height100px" />';
				$rows[$key]['title']=$data->title;
				$rows[$key]['url']=$data->url;
			}
	  	}

		$form['videos'] = array(
			'#name'=>'videos',
			'#prefix' => '<div id="divvideos"><a target="_blank" href="'.$json_url.'">'.$json_url.'</a>',
			'#suffix' => '</div>',
			'#tree' => TRUE,
			'#theme' => 'table',
			'#header' => array(/*'channel',*/'created_time',/*'embed_url','id','owner',*/'thumbnail','title','url'),
			'#rows' => $rows,
			'#attributes' => array('id' => 'tablevideos','style' => array('width:auto')),
		);
		return $form;
	}

	function _ajaxActionsUpdate($form,&$form_state){
		foreach ($form_state['values']['example_items'] as $id => $item) {
		  db_query("UPDATE {dailymotion_channels_videos} SET weight = :weight WHERE id = :id",array(':weight' => $item['weight'], ':id' => $id));
		}
		return $form['videos'];
	}

	function theme_show_videos_dailymotion($variables) {
		$form = $variables['form'];
		$rows = array();
		foreach (element_children($form['example_items']) as $id){
			$form['example_items'][$id]['weight']['#attributes']['class'] = array('example-item-weight');
			$rows[] = array(
				'data' => array(
					drupal_render($form['example_items'][$id]['name_channel']),
					drupal_render($form['example_items'][$id]['weight']),
				),
				'class' => array('draggable'),
			);
		}
		$output = theme('table', array(
			
			'header' => array(t('Ordenar Canales'), t('Weight')),
			'rows' => $rows,
			'attributes' => array('id' => 'example-items-table','style' => array('width:auto')),
		));
		$output .= drupal_render_children($form);
		drupal_add_tabledrag('example-items-table', 'order', 'sibling', 'example-item-weight');
		return $output;
	}

	function _ajaxCanalesSelect($form,&$form_state){
		return $form['videos'];
	}

	function _ajaxActionsButton($form,&$form_state){
		$id_canal = $form_state['values']['canales'];
		/*$json = file_get_contents("https://api.dailymotion.com/playlist/".$form_state['values']['canales']."/videos?fields=channel,embed_url,id,owner,thumbnail_1080_url,title,url,created_time&limit=10&sort=recent");
		$data =  json_decode($json);*/
		read_dailymontion_videos($id_canal);
		return $form['videos'];
	}
