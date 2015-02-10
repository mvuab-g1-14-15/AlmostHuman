
$(document).ready(function() 
{
	$.ajax({
            type: "GET",
            url: "log.xml",
            cache: false,
            dataType: "xml",
            success: function(xml) {

    			var trace_iteration = 0;

        		$(xml).find('log').each(function(){

        			trace_iteration = 		trace_iteration + 1;

                    var module = 			$(this).attr('module');
                    var engine_class = 		$(this).attr('engine_class');
					var msg = 				$(this).attr('msg');
					var timestamp = 		$(this).attr('timestamp');
					var date_time = 		$(this).attr('date_time');
					var level = 			$(this).attr('level');
					

//<p class="System"><span class="time">0</span><a onclick="hide('trace0')">STACK</a> GlobalManager.Awake</p>
					var item = '<p class="' + module + '"><img src="img/level_'+level+'.png" alt="'+level+'" style="width:35px;height:35px"><span class="time">'+date_time+'</span></span><a onclick="hide(\'trace_'+trace_iteration+'\')">DETAILS</a> ' + engine_class + ' </p><pre id="trace_'+trace_iteration+'"> '+ msg +'</pre>';

					 $('#logger_wrapper').append($(item));
     	 		});
            }

       
	});
});

/**
	Functions for toggling visibility in the log.
*/


/**
	Gets all the elements in the node of the given  class and tag.
*/
function getElementsByClass(searchClass,node,tag) {
	var classElements = new Array();
	
	if ( node == null )
	{
		node = document;
	}
	
	if ( tag == null )
	{
		tag = '*';
	}
	
	var elements = node.getElementsByTagName(tag);
	var elementsSize = elements.length;
	var pattern = new RegExp("(^|\\s)" + searchClass + "(\\s|$)");
	
	for (i = 0, j = 0; i < elementsSize; i++) 
	{
		if ( pattern.test(elements[i].className) ) 
		{
			classElements[j] = elements[i];
			j++;
		}
	}
	
	return classElements;
}

/**
	For toggling visibility of stacks in the HTML log.
*/
function hide(id) 
{
	var element_style = document.getElementById(id).style;
	var status = element_style.display;
	
	if (status != 'block') 
	{
		element_style.display = 'block';
	}
	else //status == visible
	{
		element_style.display = 'none';
	}
}

/**
	For toggling visibility of a class in the HTML log.
*/
function hide_class(className)
{
	var elements = getElementsByClass(className);
	
	var pattern = new RegExp("(^|\\s)Button(\\s|$)");
	
	for(i = 0; i < elements.length; i++)
	{		
		if(!pattern.test(elements[i].className))
		{
			if(elements[i].style.display != 'none')
			{
				elements[i].style.display = 'none'
			}
			else
			{
				elements[i].style.display = 'block'
			}
		}
	}
}

//http://tutorialab.com/web-deisgn-tutorials/jquery-js/79-create-dynamic-html-using-xml-and-jquery
function LoadXML() {

    /*$.get('log.xml', function(callback_data){
        
        var trace_iteration = 0;


        //loop each logger_row
        $(callback_data).find('log').each(function(){
          
          	$('#logger_wrapper').append('<div id="logger_row">');

	          var $log = $(this);

console.log ($log);

	          var module = $log.find('module').text();
	          var engine_class = $log.find('engine_class').text();
	          var msg = $log.find('msg').text();
	          var timestamp = $log.find('timestamp').text();
	          var date_time = $log.find('date_time').text(); 
	          var level = $log.find('info').text();
	          var trace_iteration = trace_iteration + 1;

	          var item = '<p class="' + engine_class + '">';
	          /* var item = '<p class="' + engine_class + '"> <a onclick="hide( trace_'+ trace_iteration +')">STACK</a> '+ module +'</p>
	          <pre id=" trace_'+ trace_iteration +'">'+msg+'</pre>';
          

          $('#logger_row').append($(item));    

        });
  	});*/

/*
	$.ajax({
            type: "GET",
            url: "log.xml",
            cache: false,
            dataType: "xmlp",
            success: function(xml) {
                $(xml).find('log').each(function(){
                    
 					var trace_iteration = 0;

					$('#logger_wrapper').append('<div id="logger_row">');	

                    var module = $log.find('module').text();
					var engine_class = $log.find('engine_class').text();
					var msg = $log.find('msg').text();
					var timestamp = $log.find('timestamp').text();
					var date_time = $log.find('date_time').text(); 
					var level = $log.find('info').text();
					var trace_iteration = trace_iteration + 1;

					var item = '<p class="' + engine_class + '">';

					 $('#logger_row').append($(item)); 
                });
            },
            setTimeout(function() {}, 0);

        });*/


}
