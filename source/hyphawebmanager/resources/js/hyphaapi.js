if (typeof jQuery === 'undefined') {
  throw new Error('requires jQuery')
}

function hCreateConnection(vhandler, vplugin) {
	$.post("/api/connections", { handler:vhandler, plugin:vplugin}, function(response,status){})
}

function hCreatePlugin(vpluginid, vtype, vhost) {
	$.post("/api/plugins", { id:vpluginid, type:vtype, host:vhost}, function(response,status){})
}
