	jQuery(document).ready(function(){
		jQuery('#owl-channels').owlCarousel({
		    stagePadding: 25,
		    loop:true,
		    margin:0,
		    nav:true,
		    pagination : false,
		    dots:false,
		    responsive:{
		        0:{
		            items:2
		        },
		        600:{
		            items:3
		        },
		        1000:{
		            items:3
		        }
		    }
		}).on("dragged.owl.carousel", function (event) {
			// console.log("event : ", event.relatedTarget["_drag"]["direction"]);
			var owlnav = jQuery("#owl-channels .owl-nav");
			if (owlnav.attr("style") != "undefined" /* && window.innerWidth < 768 */) {
			  owlnav.attr("style", "display:none;");
			  console.log("event : ", event.relatedTarget["_drag"]["direction"]);
			}
		  });

		jQuery.fn.owlvideos();

		jQuery('.thumbnail').on('click', function(event){
			jQuery.fn.thumbnailclick(jQuery(this));
		});

		jQuery('.channel').on('click', function(event){
			var $this = jQuery(this);
			var $datasrc = "";
			if($this.hasClass('clicked')){
			}else{
				jQuery("#owl-channels .owl-stage").each(function(index) {
					jQuery(this).find('.owl-item .item').removeClass("active");
				});

				$addactive = $this.parent();
				$addactive.addClass("active");
				$dataidchannel=$this.attr("data-idchannel");
				jQuery.ajax({
					url: 'video/'+$dataidchannel,
					dataType:"json",
					success: function(respuesta){
						jQuery('#player iframe').attr("src",respuesta[0]);
						jQuery("#owl-videos").remove();
						jQuery("#player").after("<div id='owl-videos' class='owl-carousel owl-theme'></div>");
						jQuery("#owl-videos").html(respuesta[1]);
						jQuery.fn.owlvideos();
						jQuery('.thumbnail').on('click', function(event){
							jQuery.fn.thumbnailclick(jQuery(this));
						});
					},
				})
			}
		});
	});

	jQuery.fn.owlvideos = function(){
		jQuery('#owl-videos').owlCarousel({
		    stagePadding: 25,
		    loop:false,
		    margin:5,
		    nav:false,
		    pagination : false,
		    dots:false,
		    responsive:{
		        0:{
		            items:2
		        },
		        600:{
		            items:4
		        },
		        1000:{
		            items:4
		        }
		    }
		});
	}
	jQuery.fn.thumbnailclick = function($this){
		var $datasrc = "";
		if($this.hasClass('clicked')){
		}else{
			jQuery("#owl-videos .owl-stage").each(function(index) {
				jQuery(this).find('.owl-item .item .thumbnail').removeClass( "active" );
			});
			$this.addClass("active");
			$datasrc=$this.attr("data-src");
			jQuery('#player iframe').attr("src",$datasrc);
		}
	}