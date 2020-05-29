<?php

	drupal_add_js('sites/all/libraries/owl-carousel/owl.carousel.min.js');
	drupal_add_css('sites/all/libraries/owl-carousel/owl.carousel.css');
	drupal_add_css('sites/all/libraries/owl-carousel/owl.theme.css');
	drupal_add_css('sites/all/libraries/owl-carousel/owl.transitions.css');

	$videohtml="";
	$salida = "";
	$salidavideos="";
	$salidachannels="";

	$queryvideos = "SELECT * FROM dailymotion_list_videos";
	$resultvideos = db_query($queryvideos);

	$querychannels = "SELECT * FROM dailymotion_channels_videos";
	$resultchannels = db_query($querychannels);

	$salida.="<div id='owl-channels'>";
	foreach ($resultchannels as $key => $value){
		$salida.= '<div class="item channels">';
			$salida.= '<div class="channel"><a href="#">'.$value->name_channel.'</a></div>';
		$salida.= '</div>';
	}
	$salida.="</div>";


	$salida.='<div id="player">';
		$salida.= '<iframe frameborder="0" src="https://www.dailymotion.com/embed/video/x73jvc8" allowfullscreen=""></iframe>';
	$salida.='</div>';
	
	$salida.="<div id='owl-videos'>";
	foreach ($resultvideos as $key => $value) {
		$salida.= '<div class="item videos">';
			//$crop=theme('image_style_url','dailymotion_thumbnail',$value->thumbnail);
			//$salida .= '<div class="thumbnail"><a href="#">'.$crop.'</a></div>';
			$salida .= '<div class="thumbnail" data-src="'.$value->embed_url.'"><img src="'.$value->thumbnail.'" width="160" height="90" /></div>';
		$salida.= '</div>';
	}
	$salida.="</div>";
	print $salida;
?>

<script>
	jQuery(document).ready(function(){

		jQuery("#owl-channels").owlCarousel({
			navigation:false,
			pagination : false,
			dots:false,
			center:true,
			items : 3,
			margin:10,
			autoWidth:true,
			responsive: true,
			responsiveClass:true,
		});

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

		jQuery('.thumbnail').on('click', function(event){
			var $this = jQuery(this);
			var $datasrc = "";
			if($this.hasClass('clicked')){
				//$this.removeAttr('style').removeClass('clicked');
			}else{
				$datasrc=$this.attr("data-src");
				console.log($datasrc);
				jQuery('#player iframe').attr("src",$datasrc);
			}
		});

	});
</script>


