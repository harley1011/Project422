setInterval(function(){
    $.get( "nodes.html", function( data ) {
    var div =$("#node-container");
        div.replaceWith(data );
});
}, 3000);