var canvas = document.getElementById("canvas");
var canvasWidth = canvas.width;
var canvasHeight = canvas.height;
var ctx = canvas.getContext("2d");
var canvasData = ctx.getImageData(0, 0, canvasWidth, canvasHeight);
var state=0; //0:mousedown or up 1:mousemove
var mode=0; //0:create 1:move&resize 2:save 3:line
var teeth=[]
var sel=-1;
var move=false;
var resize=false;
var line=false;

function setCanvasdata(width,height)
{
	canvasWidth=width;
	canvasHeight=height;
	canvasData = ctx.getImageData(0, 0, canvasWidth, canvasHeight);
}

function savebtnclick(){
    $("#savebtn").click();
}


function formsave(){
    var name=$("#name").val();
    $(".masthead-brand").html("<i class=\"fas fa-user-md\"></i> "+name);
    $("#fname").val(name);
    $("#flpp").val($("#line").val());
    if($("#line").val()=='')
        $("#flpp").val('3.5')
    $("#fteeth").val(JSON.stringify(teeth));
    $("#fimg").val($("#teeth").attr('src'));
    $("#form").submit();
}

// That's how you define the value of a pixel //
function drawPixel (x, y, r, g, b, a) {
    var index = (x + y * canvasWidth) * 4;
    canvasData.data[index + 0] = r;
    canvasData.data[index + 1] = g;
    canvasData.data[index + 2] = b;
    canvasData.data[index + 3] = a;
}

function updateCanvas() {
    //ctx.clearRect(0,0,canvas.width,canvas.height);
    var img=document.getElementById("teeth");
    ctx.drawImage(img,0,0,img.width,img.height,0,0,800,400);
    DrawBoxes();
}

function init(){
    canvas.addEventListener("mousedown",listener);
    canvas.addEventListener("mousemove",listener);
    canvas.addEventListener("mouseup",listener);
    window.addEventListener("keypress",keylistener);
    window.addEventListener("keyup",keylistener);
    updateCanvas();
    teeth.push({"start":{'X':0,'Y':0},"end":{'X':0,'Y':0}});
}

function DrawBoxes(){
    for(var i=0;i<teeth.length;i++){
        if(line==false && i==0)
        {
            continue;
        }
        var t=teeth[i];
        var w=Math.abs(t['end'].X-t['start'].X);
        var h=Math.abs(t['end'].Y-t['start'].Y);
        ctx.fillStyle=t['fill'];
        ctx.fillRect(t['start'].X,t['start'].Y,w,h );
    } 
}

function keylistener(e){
    if($("#exampleModal").css("display")!='none' || $("#exampleModal_save").css("display")!='none')
        return;

    switch(e.type){

        case "keyup":            
            if(e.which==27){ //esc
                updateCanvas();
                mode=2;
            }
            else if(e.which==46 && line!=true){ //del
                teeth[sel]['start'].X=-1;
                delbox();
                updateCanvas();
            }
            break;
        case "keypress":
            
            if(e.which==49){//1
                mode=0;
            }
            else if(e.which==50){//2
                if(teeth.length>0){
                    mode=1;
                }
            }
            else if(e.which==51){//3
                if(teeth.length>0){
                    savebox();
                    mode=2;
                }
            }
            else if(e.which==52){//4
                if(line==false){
                    line=true;
                    if(teeth[0]['start'].X==0 && teeth[0]['start'].Y==0)
                        mode=0;
                    else
                        mode=1;
                }
                else
                    line=false;
                updateCanvas();
            }
            else if(e.which==53){//5

            }
            break;
    }
}

function listener(e){

    if(mode==0)
        createbox(e);
    else if(mode==1){
        movebox(e);
        resizebox(e);
    }else if(mode==3){
        drawlinebox(e);
    }
}

function calculate(){
    var img=document.getElementById("teeth");
    ctx.drawImage(img,0,0,img.width,img.height,0,0,800,400);
    var lineheight=parseFloat(Math.abs(teeth[0]['end'].Y-teeth[0]['start'].Y));
    var lineval=parseFloat($("#line").val());
    lpp=lineval/lineheight;
    
    ctx.font = "12px Arial";
    ctx.fillStyle="black";
    for(var i=1;i<teeth.length;i++){
        ctx.beginPath();
        var w=Math.abs(teeth[i]['end'].X-teeth[i]['start'].X);
        var h=Math.abs(teeth[i]['end'].Y-teeth[i]['start'].Y);
        ctx.moveTo(teeth[i]['start'].X+w/2,teeth[i]['start'].Y);
        ctx.lineTo(teeth[i]['start'].X+w/2,teeth[i]['end'].Y);
        ctx.strokeStyle="#FF0000";
        ctx.stroke();
        var hl=lpp*h;
        hl=hl.toFixed(3);
        ctx.fillText(hl+"mm",teeth[i]['start'].X+w/2,teeth[i]['start'].Y+h/3);

        ctx.beginPath();
        ctx.moveTo(teeth[i]['start'].X,teeth[i]['start'].Y+h/2);
        ctx.lineTo(teeth[i]['end'].X,teeth[i]['start'].Y+h/2);
        ctx.strokeStyle="green";
        ctx.stroke();
        var wl=lpp*w;
        wl=wl.toFixed(3);
        ctx.fillText(wl+"mm",teeth[i]['start'].X,teeth[i]['start'].Y+h/2);
    }
}

function delbox(){
    tmp=[];
    for(var i=0;i<teeth.length;i++)
    {
        if(teeth[i]['start'].X>=0)
            tmp.push(teeth[i]);
    }
    teeth=tmp;
    sel=-1;
}

function savebox(){
    for(var i=0;i<teeth.length;i++)
        teeth[i]['save']=true;
    alert("length:"+teeth.length+" "+JSON.stringify(teeth));
    sel=-1;
    updateCanvas();

}
function isCenter(e){
    for(var i=0;i<teeth.length;i++)
    {
        ti=i;
        if(line==true)
            ti=0;
        else if(line==false && i==0)continue;
        p=teeth[ti]['start'];
        c=teeth[ti]['end'];
        touch=getPosition(e);
        if(touch.X>p.X+10 && touch.X<c.X-10 && touch.Y>p.Y+10 && touch.Y<c.Y-10)
        {
            return true;
        }
    }
    return false;
}

function isBorder(e){
    for(var i=0;i<teeth.length;i++)
    {
        ti=i;
        if(line==true)
            ti=0;
        else if(line==false && i==0)continue;
        p=teeth[ti]['start'];
        c=teeth[ti]['end'];
        touch=getPosition(e);
        if(touch.X>p.X && touch.X<c.X&&
            touch.Y>p.Y && touch.Y<c.Y)
        {
            var diff=5;
            if(touch.X<p.X+diff)return 1;
            else if(touch.Y<p.Y+diff)return 2;
            else if(touch.X>c.X-diff)return 3;
            else if(touch.Y>c.Y-diff)return 4;
        }
    }
    return -1;
}

function resizebox(e){
    if(move==true)return;
    if(e.type=="mousedown"){
        border=isBorder(e);
    }else if(e.type=="mousemove" && border>0){
            if(sel!=ti)return;        
            touch=getPosition(e);
            switch(border){
                case 1:
                    teeth[ti]['start']['X']=touch.X;
                    break;
                case 2:
                   teeth[ti]['start']['Y']=touch.Y;
                    break;
                case 3:
                    teeth[ti]['end']['X']=touch.X;
                    break;
                case 4:
                    teeth[ti]['end']['Y']=touch.Y;
                    break;
            }
            updateCanvas();
            var width=Math.abs(teeth[ti]['start'].X-teeth[ti]['end'].X);
            var height=Math.abs(teeth[ti]['start'].Y-teeth[ti]['end'].Y);
            ctx.strokeRect(teeth[ti]['start'].X,teeth[ti]['start'].Y,width,height);
        
    }else if(e.type=="mouseup" && border>0){
        border=-1;
    }
}

function movebox(e){
    if(e.type=="mousedown"){
        if(isCenter(e)==true){
            if(sel!=ti){
                p=teeth[ti]['start'];
                c=teeth[ti]['end'];
                width=Math.abs(c.X-p.X);
                height=Math.abs(c.Y-p.Y);
                updateCanvas();
                ctx.strokeRect(p.X,p.Y,width,height);
                sel=ti;
                return;
            }
            p=teeth[ti]['start'];
            c=teeth[ti]['end'];
            width=Math.abs(c.X-p.X);
            height=Math.abs(c.Y-p.Y);
            width/=2; height/=2;    
            move=true;
            return;
        }
    }else if(e.type=="mousemove" && move==true && sel==ti){
        touch=getPosition(e);
        teeth[ti]['start']={'X':touch.X-width,'Y':touch.Y-height};
        teeth[ti]['end']={'X':touch.X+width,'Y':touch.Y+height};
        updateCanvas();
        ctx.strokeRect(touch.X-width,touch.Y-height,width*2,height*2);
    }else if(e.type=="mouseup" && move==true && sel==ti){
        move=false;
    }
}

function createbox(e)
{
    if(e.type=="mousedown" && state==0){
        
        r=Math.floor(Math.random()*255);
        g=Math.floor(Math.random()*255);
        b=Math.floor(Math.random()*255);
        p=getPosition(e);
        if(line==true)//line
        {
            r=255;
            g=0;
            b=0;
            ti=0;
        }else{
            teeth.push({});
            ti=teeth.length-1;
        }
        teeth[ti]['start']=p;
        teeth[ti]['end']={"X":0,"Y":0};
        updateCanvas();
        state=1;
    }
    else if(e.type=="mousemove" && state==1){
        c=getPosition(e);
        teeth[ti]['end']=c;
        teeth[ti]['fill']="rgba("+r+","+g+","+b+",0.4)";
        updateCanvas();
        ctx.strokeStyle="#FF0000";
        ctx.strokeRect(p.X,p.Y,c.X-p.X,c.Y-p.Y);
    }
    else if(e.type=="mouseup" && state==1){
        teeth[ti]['save']=false;
        state=0;
        mode=1;
        sel=ti;
        w=Math.abs(teeth[ti]['end'].X-teeth[ti]['start'].X);
        h=Math.abs(teeth[ti]['end'].Y-teeth[ti]['start'].Y);
        if(w<20 && h<20 && ti!=0){
            teeth[ti]['start'].X=-1;
            delbox();
        }
        updateCanvas();
    }
}

function getPosition(e){
    var x=e.pageX-canvas.offsetLeft;
    var y=e.pageY-canvas.offsetTop;
    return {X:x,Y:y};
}


window.onload=init();