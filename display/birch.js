var gWidth = 0;
var gHeight = 0;

/**
 * Draws a rounded rectangle using the current state of the canvas. 
 * If you omit the last three params, it will draw a rectangle 
 * outline with a 5 pixel border radius 
 * @param {CanvasRenderingContext2D} ctx
 * @param {Number} x The top left x coordinate
 * @param {Number} y The top left y coordinate 
 * @param {Number} width The width of the rectangle 
 * @param {Number} height The height of the rectangle
 * @param {Number} radius The corner radius. Defaults to 5;
 * @param {Boolean} fill Whether to fill the rectangle. Defaults to false.
 * @param {Boolean} stroke Whether to stroke the rectangle. Defaults to true.
 */
function roundRect(ctx, x, y, width, height, radius, fill, stroke) {
  if (typeof stroke == "undefined" ) {
    stroke = true;
  }
  if (typeof radius === "undefined") {
    radius = 5;
  }
  ctx.beginPath();
  ctx.moveTo(x + radius, y);
  ctx.lineTo(x + width - radius, y);
  ctx.quadraticCurveTo(x + width, y, x + width, y + radius);
  ctx.lineTo(x + width, y + height - radius);
  ctx.quadraticCurveTo(x + width, y + height, x + width - radius, y + height);
  ctx.lineTo(x + radius, y + height);
  ctx.quadraticCurveTo(x, y + height, x, y + height - radius);
  ctx.lineTo(x, y + radius);
  ctx.quadraticCurveTo(x, y, x + radius, y);
  ctx.closePath();
  if (stroke) {
    ctx.stroke();
  }
  if (fill) {
    ctx.fill();
  }        
}



/* Layout outer names */
function displayNames(ctx,bg) {
	ctx.fillStyle = "black";
	ctx.font="12pt Helvetica";
	ctx.textAlign = "center";

	bg.layout.names = {};

	var interval = gWidth / ((bg.names.length > 0) ? bg.names.length + 2 : 1);
	var x = interval;
	var i = 0;
	for(i = 0; i<bg.names.length; i++) {
		ctx.fillText(bg.names[i], x, 15);
		bg.layout.names[bg.names[i]] = {x: x, y: 25};
		x += interval;
	}
 
	return bg;
}

function calculateExtent(node, xoff, yoff) {
	if(node.children.length == 0) {
		node.width = 40;
		node.height = 40;
		node.x = xoff;
		node.y = yoff;
		return node;
	}

	var ws = 0;
	var hs = 0;
	
	var extX = 0;
	var extY = 0;

	for(var i = 0; i<node.children.length; i++) {
		node.children[i] = calculateExtent(node.children[i], 20 + xoff + ws, yoff + 20);
		ws += node.children[i].width + 20;
		hs += node.children[i].height;
		//node.children[i].x = xoff;
		//node.children[i].y = yoff;

		if(extX < node.children[i].x + node.children[i].width)
			extX = node.children[i].x + node.children[i].width;
	
		if(extY < node.children[i].y + node.children[i].height)
			extY = node.children[i].y + node.children[i].height;

	}

	node.width = (extX + 20) - xoff;
	node.height = (extY + 20) - yoff;
	node.x = xoff;
	node.y = yoff;
	return node;
}

/* Draw nodes */

function displayNode(ctx,node) {
	//alert("" + node.x + "/" + node.y + "/" + node.width + "/" + node.height);

	if(!node.hole)
		roundRect(ctx, node.x, node.y, node.width, node.height, 10);
	else {
		ctx.fillStyle = "lightGray";
		roundRect(ctx, node.x, node.y, node.width, node.height, 10, true);
	}

	ctx.fillStyle = "black";

	ctx.fillText(node.control ? node.control : "", node.x + 12, node.y + 17);

	for(var i = 0; i<node.children.length; i++) {
		displayNode(ctx,node.children[i]);
	}
}

function displayNodes(ctx,bg) {
	bg.region.x = 20;
	bg.region.y = 75;
	bg.textAlign = 'left';
	bg.textBaseline = 'bottom';
	ctx.fillStyle = "black";
	ctx.font = "12pt Helvetica";
	
	bg.region = calculateExtent(bg.region,bg.region.x,bg.region.y);
	
	ctx.strokeStyle = "black";
	ctx.fillStyle = "white";

	displayNode(ctx,bg.region);

	return bg;
}

/* Draw links */

function displayNodeLinks(ctx,bg,node) {
	var interval = node.width / (node.links ? node.links.length + 1 : 1);

	if(!node.links)
		node.links = [];

	for(var i = 0; i<node.links.length; i++) {
		ctx.beginPath();
		var portx = node.x + interval * (i+1);
		ctx.moveTo(portx, node.y);
		var targ = bg.layout.names[node.links[i]];
		ctx.quadraticCurveTo(portx, (targ.y + node.y) / 2, targ.x, targ.y);
		ctx.stroke();
	}

	for(var i = 0; i<node.children.length; i++) {
		displayNodeLinks(ctx,bg,node.children[i]);
	}
}

function displayLinks(ctx,bg) {
	displayNodeLinks(ctx,bg,bg.region);
	

	return bg;
}


/* Initialise a drawing context */
function initCanvas(elementId) {
	var canvas = document.getElementById(elementId);
	if (canvas.getContext){
		var ctx = canvas.getContext('2d');
		gWidth = canvas.width;
		gHeight = canvas.height;
		return ctx;
	}

	alert("Error: 2D canvas context is unavailable");
	return null;
}

function displayBigraph(elementId,bg) {
	var ctx = initCanvas(elementId);

	if(!bg.layout)
		bg.layout = {};
	
	bg = displayNames(ctx,bg);
	bg = displayNodes(ctx,bg);
	bg = displayLinks(ctx,bg);
}

