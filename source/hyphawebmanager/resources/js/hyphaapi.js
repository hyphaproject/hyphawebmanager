if (typeof jQuery === 'undefined') {
  throw new Error('requires jQuery')
}

// source: http://stepansuvorov.com/blog/2014/04/jquery-put-and-delete/
jQuery.each( [ "put", "delete" ], function( i, method ) {
  jQuery[ method ] = function( url, data, callback, type ) {
    if ( jQuery.isFunction( data ) ) {
      type = type || callback;
      callback = data;
      data = undefined;
    }

    return jQuery.ajax({
      url: url,
      type: method,
      dataType: type,
      data: data,
      success: callback
    });
  };
});

function hCreateConnection(vhandler, vplugin) {
	$.post("/api/connections", { handler:vhandler, plugin:vplugin}, function(response,status){})
}

function hCreatePlugin(vpluginid, vtype, vhost) {
	$.post("/api/plugins", { id:vpluginid, type:vtype, host:vhost}, function(response,status){})
}

function hCreateHandler(vhandlerid, vtype, vhost) {
	$.post("/api/handlers", { id:vhandlerid, type:vtype, host:vhost}, function(response,status){})
}

function hUpdatePluginConfig(vid, vconfig) {
	$.put("/api/plugins", { id:vid, config:vconfig}, function(response,status){})
}

function hUpdateHandlerConfig(vid, vconfig) {
	$.put("/api/handlers", { id:vid, config:vconfig}, function(response,status){})
}
