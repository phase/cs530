function start(image) {

    // 0. Connect to a webgl canvas / javascript plumbing

    var canvas = document.getElementById("webgl_example");
    var gl = canvas.getContext("webgl");
    var scale = 1.0, s2 = 1.0
    var angle = 0.0, a2 = 0.0
    var shrx = 0.0, shrx2 = 0.0
    var shry = 0.0, shry2 = 0.0
    var xcoordinate = 0.0, x2 = 0.0
    var ycoordinate = 0.0, y2 = 0.0
    var timer = 0
    document.addEventListener('keydown', (event) => {
        if (event.key == 'ArrowRight') {
            x2 = xcoordinate + 0.1
            timer = 0
        }
        if (event.key == 'ArrowLeft') {
            x2 = xcoordinate - 0.1
            timer = 0
        }
        if (event.key == 'ArrowUp') {
            y2 = ycoordinate + 0.1
            timer = 0
        }
        if (event.key == 'ArrowDown') {
            y2 = ycoordinate - 0.1
            timer = 0
        }
        if (event.key == 's') {
            s2 = scale - 0.1
            timer = 0
        }
        if (event.key == 'S') {
            s2 = scale + 0.1
            timer = 0
        }
        if (event.key == '3') {
            s2 = scale * 2.0
            timer = 0
        }
        if (event.key == '4') {
            s2 = scale * 0.5
            timer = 0
        }
        if (event.key == 'r') {
            a2 = angle - 0.1
            timer = 0
        }
        if (event.key == 'R') {
            a2 = angle + 0.1
            timer = 0
        }
        if (event.key == '1') {
            a2 = angle - 1.5708
            timer = 0
        }
        if (event.key == '2') {
            a2 = angle + 1.5708
            timer = 0
        }
        if (event.key == 'r') {
            a2 = angle - 0.1
            timer = 0
        }
        if (event.key == 'h') {
            shrx2 = shrx + 0.1
            shry2 = shry + 0.1
            timer = 0
        }
        if (event.key == 'H') {
            shrx2 = shrx - 0.1
            shry2 = shry - 0.1
            timer = 0
        }
    });
    // 1. Specify geometry / coordinates

    // X
    // |\
    // | \
    // X--X

    var vertices = [
        -0.5, 0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0,
        0.5, -0.5, 0.0,
        0.5, 0.5, 0.0];

    var indices = [0, 1, 2, 3, 4, 5];

    var texture_coordinates = [
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0
    ];

    // 2. Set texture options

    var texture_object = gl.createTexture();

    gl.bindTexture(gl.TEXTURE_2D, texture_object);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1); // hack
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

    // 3. Load the vertex buffer

    var vertex_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null); // good practice?

    // 4. Load the index buffer

    var index_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, index_buffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null); // good practice?

    // 5. Load the texture coordinate buffer

    var texture_coord_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, texture_coord_buffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texture_coordinates), gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null); // good practice?

    // 6. Specify and compiler the vertex shader

    var vertex_source =
        "precision mediump float; " + // precision
        "uniform mat4 translation; " +
        "uniform mat4 rotation; " +
        "uniform mat4 scale; " +
        "uniform mat4 sheer; " + // NOTE: pay attention
        "attribute vec3 coordinate; " +
        "attribute vec2 texture_coordinate; " +
        "varying vec2 tex;" +
        "void main(void) {" +
        "  gl_Position = translation * scale * rotation * sheer * vec4(coordinate, 1.0);" + // See here for example..
        "  tex = texture_coordinate; " +
        "}";

    var vertex_shader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertex_shader, vertex_source);
    var o = gl.compileShader(vertex_shader);
    // TODO: look at o for errors

    // 7. Specify and compiler the fragment shader

    var fragment_source =
        "precision mediump float; " + // precision
        "varying vec2 tex;" +
        "uniform sampler2D u_sampler;" +
        "void main(void) {" +
        "  gl_FragColor = texture2D(u_sampler, tex);" +
        "}";

    var fragment_shader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragment_shader, fragment_source);
    var o = gl.compileShader(fragment_shader);
    // TODO: look at o for errors

    // 8. Create a GLSL program

    var shader_program = gl.createProgram();
    gl.attachShader(shader_program, vertex_shader);
    gl.attachShader(shader_program, fragment_shader);
    gl.linkProgram(shader_program);
    // TODO: look at o for errors
    gl.useProgram(shader_program);

    // 9. Connect the variables in the program to data

    gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, index_buffer);
    var coord = gl.getAttribLocation(shader_program, "coordinate");
    gl.vertexAttribPointer(coord, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(coord);

    gl.bindBuffer(gl.ARRAY_BUFFER, texture_coord_buffer);
    var texture_coord = gl.getAttribLocation(shader_program, "texture_coordinate");
    gl.vertexAttribPointer(texture_coord, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(texture_coord);

    var u_sampler = gl.getUniformLocation(shader_program, "u_sampler");
    gl.uniform1i(u_sampler, 0);

    //    # 10. Create the render loop

    function lerp(a, b, t) { return a + (b - a) * t; }

    function draw(t) {
        t /= 500;
        xcoordinate = lerp(xcoordinate, x2, timer);
        ycoordinate = lerp(ycoordinate, y2, timer);
        scale = lerp(scale, s2, timer);
        angle = lerp(angle, a2, timer);
        shrx = lerp(shrx, shrx2, timer);
        shry = lerp(shry, shry2, timer);
        gl.uniformMatrix4fv(
            gl.getUniformLocation(shader_program, "translation"), false, ([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, xcoordinate, ycoordinate, 0, 1])
        );
        gl.uniformMatrix4fv(
            gl.getUniformLocation(shader_program, "scale"), false, ([scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1])
        );
        gl.uniformMatrix4fv(
            gl.getUniformLocation(shader_program, "rotation"), false, ([Math.cos(angle), Math.sin(angle), 0, 0, -Math.sin(angle), Math.cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1])
        );
        gl.uniformMatrix4fv(
            gl.getUniformLocation(shader_program, "sheer"), false, ([1, shrx, 0, 0, shry, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1])
        );

        timer = Math.min(1.0, timer + 0.01);

        // 11. Render

        gl.clearColor(0.8, 0.8, 0.8, 1.0);
        gl.enable(gl.DEPTH_TEST);
        gl.clear(gl.COLOR_BUFFER_BIT);
        gl.viewport(0,0,canvas.width, canvas.height);
        // Draw triangle
        gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_SHORT, 0);
    }

    function next_frame(time) {
        draw(time);
        requestAnimationFrame(next_frame);
    }
    requestAnimationFrame(next_frame);
}

var image = new Image();
image.onload = function () {start(image);};
image.src = "sample.png";
