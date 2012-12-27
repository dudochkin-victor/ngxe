<?php
error_reporting(E_ALL ^ E_NOTICE);

session_start();

define("DATABASE_NAME","sangilen");
define("DATABASE_USER","sangilen");
define("DATABASE_PASSWORD", "g90pw605");
//define("DATABASE_USER","root");
//define("DATABASE_PASSWORD",null);
define("DATABASE_HOST","localhost");

$app_path = "/";

$toptemplate_filename = "./res/template.html";
$printtemplate_filename = "./res/printtemplate.html";
require_once 'common.php';
require_once 'lib.php';
header('Content-Type: text/html; charset=utf-8');

$anounces = array();
$article = '';

function default_handle()
{
	global $db;
	global $anounces;
	
	$path = $_SERVER['PATH_INFO'];
	// По тоопливу
	$query = "SELECT * FROM articles";
	if (($path != '/') && ($path != NULL)) 
		$query .= " WHERE alias=".tosql($path, "Text");
	else
		$query .= " ORDER BY parentid ASC, created DESC";
	$db->query($query);
	if($db->next_record())
	{  		
		// Show main table based on SQL query
		do
		{
			if ($db->f("parentid") == 0)
				$anounces[$db->f("id")] = array("title" => str_replace("\t", "", $db->f("title")),
						"alias" => mysql_real_escape_string(str_replace("\t", "", $db->f("alias"))),
						"sectionid" => intval($db->f("sectionid")),
						"introtext" => str_replace("\t", "", $db->f("introtext")),
						//"fulltext" => str_replace("\t", "", $db->f("fulltext"))
						);
			else
			{
				if ($anounces[$db->f("parentid")]["childs"] == null)
					$anounces[$db->f("parentid")]["childs"] = array();
				$anounces[$db->f("parentid")]["childs"][$db->f("id")] = array("title" => str_replace("\t", "", $db->f("title")),
						"alias" => mysql_real_escape_string(str_replace("\t", "", $db->f("alias"))),
						"sectionid" => intval($db->f("sectionid")),
						"introtext" => str_replace("\t", "", $db->f("introtext")),
						//"fulltext" => str_replace("\t", "", $db->f("fulltext"))
						);
			}
		} while($db->next_record());
	}
	else
	{
		echo 'Данные не найдены';
	}
}

function handle_post()
{
	$method = strtolower($_POST['method']);
	if (!isset($_SESSION['login']) && $method != 'login')
	{
		require_once 'login.html';
	}else
	{
		$user_rights = $_SESSION['user_rights'];
		switch ($method){
		case 'login':
			handle_login();
		break;
		case 'fuelsale':
			handle_fuelsale();
		break;
		case 'cardsale':
			handle_cardsale();
		break;
		case 'details_report':
			if ($user_rights == 2) handle_details_report();
		break;
		case 'user_report':
			if ($user_rights == 2) handle_user_report();
		break;
		case 'fuel_report':
			if ($user_rights == 2) handle_fuel_report();
		break;
		case 'weekday_report':
			if ($user_rights == 2) handle_weekday_report();
		break;
		case 'time_report':
			if ($user_rights == 2) handle_time_report();
		break;
		case 'client_report':
			if ($user_rights == 2) handle_client_report();
		break;
		default:
		
		break;
		}
	}		
}
 
function handle_get()
{
	default_handle();
}

$method = $_SERVER['REQUEST_METHOD'];
switch ($method) {
  case 'POST':
    handle_post();  
    break;
  case 'GET':
    handle_get();  
    break;
  default:
    break;
}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>ЗАО Сангилен+</title>
<link href="css/bootstrap.css" rel="stylesheet">
<link rel="stylesheet" href="css/style.css" type="text/css" media="screen"/>
<script type="text/javascript" src="js/jquery-1.6.js"></script>
<script type="text/javascript" src="js/jquery.form.js"></script>
<script type="text/javascript" src="js/jquery.tablesorter.min.js"></script>
<script type="text/javascript" src="js/bootstrap-modal.js"></script>
<script type="text/javascript" src="js/bootstrap-dropdown.js"></script>
<script type="text/javascript" src="js/bootstrap-twipsy.js"></script>
<script type="text/javascript" src="js/bootstrap-scrollspy.js"></script>
<script type="text/javascript" src="js/bootstrap-tabs.js"></script>
<script type="text/javascript">
	$(document).ready(function() {
		window.onresize = onResize;
		onResize();
		//select all the a tag with name equal to modal
		$('a[name=modal]').click(function(e) {
			//Cancel the link behavior
			e.preventDefault();

			//Get the A tag
			var id = $(this).attr('href');

			//Get the screen height and width
			var maskHeight = $(document).height();
			var maskWidth = $(window).width();

			//Set heigth and width to mask to fill up the whole screen
			$('#mask').css({
				'width' : maskWidth,
				'height' : maskHeight
			});

			//transition effect		
			$('#mask').fadeIn(1000);
			$('#mask').fadeTo("slow", 0.8);

			//Get the window height and width
			var winH = $(window).height();
			var winW = $(window).width();

			//Set the popup window to center
			$(id).css('top', winH / 2 - $(id).height() / 2);
			$(id).css('left', winW / 2 - $(id).width() / 2);

			//transition effect
			$(id).fadeIn(2000);
		});

		//if close button is clicked
		$('.window .modal-close').click(function(e) {
			//Cancel the link behavior
			e.preventDefault();
			$('#mask').hide();
			$('.window').hide();
		});

		//if mask is clicked
		/*$('#mask').click(function() {
			$(this).hide();
			$('.window').hide();
		});*/

	});

	function onResize() {
		var windowHeight = $(window).height();
		var windowWidth = $(window).width();
		$('#content').css({
			'width' : windowWidth,
			'height' : windowHeight,
			'overflow' : 'hidden'
		});
		$('.referencecontainer').css({
			'width' : windowWidth-40
		});
		$('#st_main').css({
			'width' : windowWidth,
			'height' : windowHeight,
			
		}); 
	}

	function jsonRPC(obj, onData, onError) {
		$.ajax({
			contentType : 'application/json',
			data : JSON.stringify(obj),
			dataType : 'json',
			success : onData,
			error : onError,
			processData : false,
			type : 'POST',
			url : '/jsonrpc/'
		});
		return false;
	}
</script>
<style type="text/css">

html {
	background-color:#000000;
	height: 100%; 
	width: 100%;
}

	span.reference{
		font-family:Arial;
		//position:fixed;
		//left:10px;
		//bottom:10px;
		//font-size:11px;
	}
	span.reference a{
		color:#aaa;
		text-decoration:none;
		margin-right:20px;
	}
	span.reference a:hover{
		color:#ddd;
	}
</style>

</head>
<body style="background-color: #000000; height: 100%; width: 100%;">
	<div class="topbar" data-scrollspy="scrollspy">
		<div class="topbar" data-dropdown="dropdown">
			<div class="topbar-inner">
				<div class="container">
					<h3>
						<a href="#"><img height=30px src="logo.png"></a>
					</h3>
					<ul class="nav">
						<li class="active"><a href="#">Главная</a></li>
						<li class="dropdown"><a href="#" class="dropdown-toggle">Новости</a>
							<ul class="dropdown-menu">
								<?php 
									foreach ($anounces as &$anounce) {
										$sectionid = $anounce['sectionid'];
										$title = $anounce['title'];
										$alias = $anounce['alias'];
										if ($sectionid == 0)
											echo "<li><a href=\"\" data-controls-modal=\"modal$alias\"
													data-backdrop=\"true\" data-keyboard=\"true\">$title</a></li>";
									}
								?>
							</ul></li>
						<li class="dropdown"><a href="#" class="dropdown-toggle">Деятельность</a>
							<ul class="dropdown-menu">
								<?php 
									foreach ($anounces as &$anounce) {
										$sectionid = $anounce['sectionid'];
										$title = $anounce['title'];
										$alias = $anounce['alias'];
										if ($sectionid == 1)
											echo "<li><a href=\"\" data-controls-modal=\"modal$alias\"
													data-backdrop=\"true\" data-keyboard=\"true\">$title</a></li>";
									}
								?>
							</ul></li>
						<li class="dropdown"><a href="#" class="dropdown-toggle">Галлерея</a>
							<ul class="dropdown-menu">
								<?php 
									foreach ($anounces as &$anounce) {
										$sectionid = $anounce['sectionid'];
										$title = $anounce['title'];
										$alias = $anounce['alias'];
										if ($sectionid == 2)
											echo "<li><a href=\"\" data-controls-modal=\"modal$alias\"
													data-backdrop=\"true\" data-keyboard=\"true\">$title</a></li>";
									}
								?>
							</ul></li>
					</ul>
					<form class="pull-left" action="">
						<input placeholder="Search" type="text">
					</form>
					<ul class="nav secondary-nav">
						<li><a href="" data-controls-modal="modal-contacts"
							data-backdrop="true" data-keyboard="true">Контакты</a></li>
						<li><a href="" data-controls-modal="modal-response"
							data-backdrop="true" data-keyboard="true">Написать нам</a></li>
						<li><a href="/flash/3/1.swf">3D-экскурсия</a></li>
					</ul>
				</div>
			</div>
			<!-- /topbar-inner -->
		</div>
		<!-- /topbar -->
	</div>
	<!-- /topbar-wrapper -->

	<script type="text/javascript">
		function onData(data) {
			console.log("device control succeeded");
		}
		function onError() {
			console.log("Device control failed");
		}
		function test() {
			jsonRPC({
				"method" : "mobile.locate",
				"params" : [ "фыв", "фыв", "werовадпо" ],
				"id" : "id value (optional)"
			}, onData, onError);
		}
	</script>
	<div id="content">
		
		<!-- a href="" onclick="return test();"> send</a-->
		<div id="st_main" class="st_main">
			<img src="images/album/1.jpg" alt="" class="st_preview" style="display:none;"/>
			<div class="st_overlay"></div>
			<br/><br/>
			<h1>Gallery</h1>
			<!-- div id="st_loading" class="st_loading"><span>Loading...</span></div-->
			<ul id="st_nav" class="st_navigation" style="display : none;">
				<li class="album">
					<span class="st_link">Photos<span class="st_arrow_down"></span></span>
					<div class="st_wrapper st_thumbs_wrapper">
						<div class="st_thumbs">
							<img src="images/album/thumbs/1.jpg" alt="images/album/1.jpg"/>
							<img src="images/album/thumbs/2.jpg" alt="images/album/2.jpg"/>
							<img src="images/album/thumbs/3.jpg" alt="images/album/3.jpg"/>
							<img src="images/album/thumbs/4.jpg" alt="images/album/4.jpg"/>
							<img src="images/album/thumbs/1.jpg" alt="images/album/1.jpg"/>
							<img src="images/album/thumbs/2.jpg" alt="images/album/2.jpg"/>
							<img src="images/album/thumbs/3.jpg" alt="images/album/3.jpg"/>
							<img src="images/album/thumbs/4.jpg" alt="images/album/4.jpg"/>
							<img src="images/album/thumbs/1.jpg" alt="images/album/1.jpg"/>
							<img src="images/album/thumbs/2.jpg" alt="images/album/2.jpg"/>
							<img src="images/album/thumbs/3.jpg" alt="images/album/3.jpg"/>
							<img src="images/album/thumbs/4.jpg" alt="images/album/4.jpg"/>
						</div>
					</div>
				</li>
			</ul>
		</div>
	</div>
        <div class="referencecontainer">
            <span class="reference">
                <a href="/flash">Flash-версия сайта</a>
            </span>
		</div>
	<?php
		foreach ($anounces as &$anounce) {
			$title = $anounce['title'];
			$alias = $anounce['alias'];
			$introtext = $anounce['introtext'];
			$active_part = 0;
			if ($anounce["childs"] != null)
				echo "<div id=\"modal$alias\" class=\"modal hide fade\" style=\"width:70%; left:30%; margin-left:-15%;\">
				<div class=\"modal-header\">
					<a href=\"#\" class=\"close\">&times;</a>
					<h3 id=\"modal-header\">$title</h3>";
			else
				echo "<div id=\"modal$alias\" class=\"modal hide fade\">
				<div class=\"modal-header\">
					<a href=\"#\" class=\"close\">&times;</a>
					<h3 id=\"modal-header\">$title</h3>";
					if ($anounce["childs"] != null)
					{
						echo "<ul class=\"tabs\">";
							$active = true;
							foreach ($anounce["childs"] as &$part) {
								$part_title = $part["title"];
								$part_alias = $part["alias"];
								if ($active)
								{
									$active_part = $part_alias;
									echo "<li class=\"active\" onclick=\"return showPart($part_alias, $active_part);\"><a href=\"#\">$part_title</a></li>";
								}
								else
					        		echo "<li onclick=\"return showPart($part_alias, $active_part);\"><a href=\"#\">$part_title</a></li>";
					        	$active = false;
					        }
				        echo "</ul>";
					}
			echo"</div>
				<div id=\"modal-body\" class=\"modal-body\">";
				if ($anounce["childs"] != null)
				{
					$active = true;
					foreach ($anounce["childs"] as &$part) {
						$part_intro = $part["introtext"];
						$part_alias = $part["alias"];
						if ($active)
							echo "<div id=\"part$part_alias\">$part_intro</div>";
						else
			        		echo "<div id=\"part$part_alias\" class=\"hide\">$part_intro</div>";
			        	$active = false;
					}
				}
				else 
					echo $introtext;
				echo "</div>
				<div class=\"modal-footer\">
					<a href=\"\" class=\"btn primary\" onClick=\"return hideModal('modal$alias', $active_part);\">Ok</a>
				</div>
			</div>";
		}
	 ?>
			<style>
			#scaled {
			    width: 100%;
			    display: block;
			    float: left;
			}

			#scaled img{
			    vertical-align: middle;
			    max-width: 100%;
			}
			.img {
			    -moz-transform: scale(0.8);
			    -moz-transition-duration: 0.4s;
			    background-color: #F0F0F0;
			    float: left;
			    padding: 4px;
			    -webkit-border-radius: 4px;
			    -moz-border-radius: 4px;
			    border-radius: 4px;
			}
			.img:hover {
			    -moz-transform: scale(1);
			    box-shadow: 0 0 15px #888888;
			    -webkit-border-radius: 4px;
			    -moz-border-radius: 4px;
			    border-radius: 4px;
			}
			</style>
			<div id="modal-contacts" class="modal hide fade" style="width:70%; left:30%; margin-left:-15%;" >
				<div class="modal-header">

					<a href="#" class="close">&times;</a>
					<h3 id="modal-header">Контакты</h3></div>
				<div id="modal-body" class="modal-body">
			<div title="Контакты"	class="row">
			<div style="float: left;">
				<div class="img">
					<a title="Гольдман Роман Генадьевич" href="http://gogoo.ru/id00000006/photo/album/devki"><img alt="" src="http://gogoo.ru/img/photo_album.png"/></a>
					<p>
						<strong>Гольдман Роман Генадьевич</strong><br/>
						Генеральный директор<br/>
						<strong>Контакты:</strong><br/>
						<strong>Тел:</strong> 8(391) 2777-444<br/>
						<strong>skype:</strong> goldman_r<br/>
						<strong>Email:</strong> goldman@sangilen.ru<br/>
						<strong>www:</strong> Блог Гольдмана "Сибирское агентство новостей"
					</p>

				</div>
				<div class="img">
					<a title="Гольдман Роман Генадьевич" href="http://gogoo.ru/id00000006/photo/album/devki"><img alt="" src="http://gogoo.ru/img/photo_album.png"/></a>
					<p>
						<strong>Гольдман Роман Генадьевич</strong><br/>
						Генеральный директор<br/>
						<strong>Контакты:</strong><br/>
						<strong>Тел:</strong> 8(391) 2777-444<br/>
						<strong>skype:</strong> goldman_r<br/>
						<strong>Email:</strong> goldman@sangilen.ru<br/>
						<strong>www:</strong> Блог Гольдмана "Сибирское агентство новостей"
					</p>

				</div>
				</div>
			</div>
			</div>
				<div class="modal-footer">
					<a href="" class="btn primary" onClick="return hideModal('modal-contacts', 0);">Отправить</a>
				</div>
			</div>
			<div id="modal-response" class="modal hide fade">
				<div class="modal-header">

					<a href="#" class="close">&times;</a>
					<h3 id="modal-header">Обратная связь</h3></div>
				<div id="modal-body" class="modal-body">
			<div title="Связаться"	class="row">
			<div class="span4">
				<h3>Сангилен+</h3>
				<p>КОНТАКТНАЯ ИНФОРМАЦИЯ<br/>
				T. +7(391) 2-777-444<br/>
				<a href="mailto:info@sangilen.ru">info@sangilen.ru</a><br/>
				<strong>Адрес:</strong><br/> 
				г.Красноярск,<br/>
				ул. Давыдова, 66</p>
			</div>
			<div class="span4">
				<input id="cname" style="width: 100%; min-height: 30px;"><br/>
				<input id="cmail" style="width: 100%; min-height: 30px;"><br/>
				<textarea id="cmessage" class="boxsizingBorder"
					style="width: 100%; height: 110px; margin-bottom: 10px; min-height: 110px; overflow-y: auto; margin-left: 2px; padding-left: 2px; border: 1px solid #CCCCCC;">
				</textarea>
			</div>
			</div></div>
				<div class="modal-footer">
					<a href="" class="btn primary" onClick="return hideModal('modal-response', 0);">Отправить</a>
				</div>
			</div>
	<script type="text/javascript">
		var active_part = -1;
		function hideModal(obj, active) {
			$('#part'+active_part).hide();
			$('#part'+active).show();
			active_part = -1;
			$('#'+obj).modal('hide');
			return false;
		}
		function showPart(obj, active) {
			if (active_part == -1)
				active_part = active;
			$('#part'+active_part).hide();
			$('#part'+obj).show();
			active_part = obj;
			return false;
		}
		
		function tableClick() {
			$('#usertable').toggle('fast');
			onResize();
			/*if ($('#usertable').is(":visible"))
				$('#usertable').fadeOut(300);
			else
				$('#usertable').fadeIn(300);*/
		}
		$(function() {
			$("table#sortTableExample").tablesorter({
				sortList : [ [ 1, 0 ] ]
			});
		});
		
		$(function() {
			//the loading image
			//var $loader		= $('#st_loading');
			//the ul element 
			var $list		= $('#st_nav');
			//the current image being shown
			var $currImage 	= $('#st_main').children('img:first');
			
			//let's load the current image 
			//and just then display the navigation menu
			$('<img>').load(function(){
				$currImage.fadeIn(3000);
				//slide out the menu
				setTimeout(function(){
					//$list.animate({'left':'-20px'},500);
					//$loader.hide();
				},
				1000);
			}).attr('src',$currImage.attr('src'));
			
			//calculates the width of the div element 
			//where the thumbs are going to be displayed
			//buildThumbs();
			
			function buildThumbs(){
				$list.children('li.album').each(function(){
					var $elem 			= $(this);
					var $thumbs_wrapper = $elem.find('.st_thumbs_wrapper');
					var $thumbs 		= $thumbs_wrapper.children(':first');
					//each thumb has 180px and we add 3 of margin
					var finalW 			= $thumbs.find('img').length * 183;
					$thumbs.css('width',finalW + 'px');
					//make this element scrollable
					makeScrollable($thumbs_wrapper,$thumbs);
				});
			}
			
			//clicking on the menu items (up and down arrow)
			//makes the thumbs div appear, and hides the current 
			//opened menu (if any)
			$list.find('.st_arrow_down').live('click',function(){
				var $this = $(this);
				hideThumbs();
				$this.addClass('st_arrow_up').removeClass('st_arrow_down');
				var $elem = $this.closest('li');
				$elem.addClass('current').animate({'height':'170px'},200);
				var $thumbs_wrapper = $this.parent().next();
				$thumbs_wrapper.show(200);
			});
			$list.find('.st_arrow_up').live('click',function(){
				var $this = $(this);
				$this.addClass('st_arrow_down').removeClass('st_arrow_up');
				hideThumbs();
			});
			
			var waitId = setInterval(animate, 5000);
			var imgs = $list.find('.st_thumbs img');
			var idx = 0;
			function animate()
			{
				var $this = $(imgs[idx]);
				idx++;
				if (idx>=imgs.length)
					idx = 0;
				//$loader.show();
				$('<img class="st_preview"/>').load(function(){
					var $this = $(this);
					var $currImage = $('#st_main').children('img:first');
					$this.insertBefore($currImage);
					
					$currImage.fadeOut(2000,function(){							
						//$loader.hide();
						$(this).remove();
					});
				}).attr('src',$this.attr('alt'));
					
			}
			//clicking on a thumb, replaces the large image
			$list.find('.st_thumbs img').bind('click',function(){
				var $this = $(this);
				//$loader.show();
				$('<img class="st_preview"/>').load(function(){
					var $this = $(this);
					var $currImage = $('#st_main').children('img:first');
					$this.insertBefore($currImage);
					
					hideThumbs();
					$arrow = $list.find('.st_arrow_down');
					$arrow.addClass('st_arrow_down').removeClass('st_arrow_up');
					$currImage.fadeOut(2000,function(){							
						//$loader.hide();
						$(this).remove();
					});
				}).attr('src',$this.attr('alt'));
			}).bind('mouseenter',function(){
				$(this).stop().animate({'opacity':'1'});
			}).bind('mouseleave',function(){
				$(this).stop().animate({'opacity':'0.7'});
			});
			
			//function to hide the current opened menu
			function hideThumbs(){
				$list.find('li.current')
					 .animate({'height':'50px'},400,function(){
						$(this).removeClass('current');
					 })
					 .find('.st_thumbs_wrapper')
					 .hide(200)
					 .andSelf()
					 .find('.st_link span')
					 .addClass('st_arrow_down')
					 .removeClass('st_arrow_up');
			}

			//makes the thumbs div scrollable
			//on mouse move the div scrolls automatically
			function makeScrollable($outer, $inner){
				var extra 			= 800;
				//Get menu width
				var divWidth = $outer.width();
				//Remove scrollbars
				$outer.css({
					overflow: 'hidden'
				});
				//Find last image in container
				var lastElem = $inner.find('img:last');
				$outer.scrollLeft(0);
				//When user move mouse over menu
				$outer.unbind('mousemove').bind('mousemove',function(e){
					var containerWidth = lastElem[0].offsetLeft + lastElem.outerWidth() + 2*extra;
					var left = (e.pageX - $outer.offset().left) * (containerWidth-divWidth) / divWidth - extra;
					$outer.scrollLeft(left);
				});
			}
        });
	</script>
</body>
</html>
