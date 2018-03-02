var wh=-1;


function init(){
	$.get("t_select.php",function(d,s){
		d=JSON.parse(d);
		$("#w").css("top",d.w+"px");
		$("#h").css("left",d.h+"px");
		writepos();
	});
}
window.onload=init();

function moveline(e){
	if(wh==0){//w
		if(e.clientY>300)return;
		$("#w").css("top",e.clientY+"px");
		wh=-1;
		press("#kbh",1);
		writepos();
	}else if(wh==1){//h
		if(e.clientX>200)return;
		$("#h").css("left",e.clientX+"px");
		wh=-1;
		press("#kbv",1);
		writepos();
	}
}

function writepos(){
	var w=$("#w").css("top");
	var h=$("#h").css("left");
	w=w.substring(0,w.length-2);
	h=h.substring(0,h.length-2);
	w=w/200*10;
	h=h/300*15;
	$("#pos").html(w+"mm , "+h+"mm");
}

function keyup(e){
	if(e.which==27){
		allpressup();
		wh=-1;
	}
}

function key(e){
	allpressup();
	var c=e.which;
	if(c==104){
		wh=0;
		press("#kbh");
	}else if(c==118){
		wh=1;
		press("#kbv");
	}else if(c==13){
		wh=-1;
		press("#kbe");
		save();
	}
}

function save(){
	var w=$("#w").css("top");
	var h=$("#h").css("left");
	w=w.substring(0,w.length-2);
	h=h.substring(0,h.length-2);
	$.post("t_update.php",{
		w:w,
		h:h
	},function(d,s){
		dispalarm();
	});
	
}

function dispalarm(){
	$("#alarm").slideDown("fast",function(){
		setTimeout(function(){$("#alarm").slideUp("fast",function(){press("#kbe",1)});},2000);
	});
}

function press(id,option){
	if(!option)
		$(id).css("background-color","#fda537");
	else
		$(id).css("background-color","white");
}
function allpressup(){
	press("#kbv",1);
	press("#kbh",1);
	press("#kbe",1);
}


