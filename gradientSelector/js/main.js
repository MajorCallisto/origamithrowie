/*
Christopher Lewis
SASS Example
for Christopher Lewis
March 13, 2017
INM320
555-555-555

*/
$().ready(function(){
  initEvents();
  initPallete();
  initGradient();
});
function initEvents(){
  window.addEventListener("click", updateTextarea);
}
function initPallete(){
  $("#colorpicker").spectrum({
      flat: true,
      showInput: false,
      allowEmpty:false,
      showAlpha:false,
      showSelectionPalette:true
  });
}
function initGradient(){
  gradX("#gradX", {
      targets: [".target"],
      type: "linear",
      code_shown: false,
      sliders:[{color:"#FFCCCC", position:"0"}, {color:"#FFCCFF", position:"100"}]
  });
  updateTextarea();
  $(".gradx_slider").click(function(e){})
  $("#gradx_panel_gradX").click(function(e){
    var percentage = Math.floor((e.pageX -$(this).offset().left)/$(this).width()*100);
    var selectedColor = $("#colorpicker").spectrum("get").toRgb();
    gradx.add_slider([
        {
            color: "rgb("+selectedColor.r+","+selectedColor.g+","+selectedColor.b+")",
            position: percentage //no % symbol
        }
    ]);
    gradx.apply_style(gradx.panel, gradx.get_style_value());
    gradx.update_style_array();
    gradx.apply_style(gradx.panel, gradx.get_style_value());
    gradx.update_style_array();
    updateTextarea();
  })
}
function updateTextarea(){
  var cols = gradx.sliders;//Create a reference
  cols.sort(function(a, b) {
    return a[1] - b[1];
  });
  var textStr = "#define NUMCOLORS "+cols.length+"\n";
  textStr +="OrigamiCubeColor cubeColors[NUMCOLORS] = {\n"
  var r, g, b, timing;
  for (var i in cols){
    timing = (i<cols.length-1)?Math.floor(cols[Number(i)+1][1]/100*$("#time").val()*1000)-Math.floor(cols[Number(i)][1]/100*$("#time").val()*1000):0;

    stripColor = cols[i][0].replace("rgb(","").replace(")","").split(",");
    r = stripColor[0];
    g = stripColor[1];
    b = stripColor[2];

    //If the first timer isn't at the begining, add a colour entry at 0 that's the same as the first colour
    if (i==0 && timing > 0){
      //textStr += "\tOrigamiCubeColor("+r+","+g+","+b+",0)"+((i < cols.length-1)?",":"")+"\n";
    }


    //If the last slider isn't at the end, add a timer at the end of the same colour as the last slider
    if (i==cols.length - 1){
      timing = $("#time").val()*1000 - cols[i][1]/100*$("#time").val()*1000;
    }
    textStr += "\tOrigamiCubeColor("+r+","+g+","+b+","+timing+")"+((i < cols.length-1)?",":"")+"\n";

  }
  textStr += "};";
  $("#codeOutput").val(textStr);
  console.log($("#codeOutput").val());
}
