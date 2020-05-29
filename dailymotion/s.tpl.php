<?php

drupal_add_js('sites/all/libraries/owl-carousel/owl.carousel.min.js');
drupal_add_css('sites/all/libraries/owl-carousel/owl.carousel.css');
drupal_add_css('sites/all/libraries/owl-carousel/owl.theme.css');
drupal_add_css('sites/all/libraries/owl-carousel/owl.transitions.css');

	$idowner=arg(1,drupal_get_path_alias());

	$queryvideos = "SELECT * FROM dailymotion_list_videos where owner = '".$idowner."'";
	$resultvideos = db_query($queryvideos);
	foreach ($resultvideos as $key => $value){
		$salida.= '<div class="item videos">';
			$salida .= '<div class="thumbnail" data-src="'.$value->embed_url.'"><img src="'.$value->thumbnail.'" width="140" height="98" /></div>';
		$salida.= '</div>';
	}

	print $salida;
?>	

<script>

	jQuery("#owl-videos").removeAttr('class');
	jQuery("#owl-videos").removeAttr('style');

	jQuery("#owl-videos").owlCarousel({
		navigation:false,
		pagination : false,
		dots:false,
		center:true,
		items : 7,
		lazyLoad : true,
		margin:5,
		autoWidth:true,
		responsive: true,
		responsiveClass:true,
	});


</script>