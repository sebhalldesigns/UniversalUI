#include "../include/uRenderNode.h"
#include "../include/uCanvas.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image/stb_image.h"

std::vector<uRenderCommand> DrawPoints(std::vector<Point*> points);
std::vector<uRenderCommand> DrawRectangles(std::vector<Rectangle*> rectangles);
std::vector<uRenderCommand> DrawImages(std::vector<Image*> images);

void uRenderNode::UpdateRenderCommands(uCanvas& canvas) {
    renderCommands.clear();

    std::vector<uRenderCommand> points = DrawPoints(canvas.points);
    std::vector<uRenderCommand> rectangles = DrawRectangles(canvas.rectangles);
    std::vector<uRenderCommand> images = DrawImages(canvas.images);

    renderCommands.insert(renderCommands.end(), points.begin(), points.end());
    renderCommands.insert(renderCommands.end(), rectangles.begin(), rectangles.end());
    renderCommands.insert(renderCommands.end(), images.begin(), images.end());

}

std::vector<uRenderCommand> DrawPoints(std::vector<Point*> points) {
    std::vector<uRenderCommand> commandBuffer;

    


    // TODO optimise by only adding changes to color and size if this point is differnet to the previous
    for (Point* point : points) {
        commandBuffer.push_back({ COLOR_4F, { point->color.r, point->color.g, point->color.b, point->color.a } });
        commandBuffer.push_back({ POINT_SIZE, { point->radius/2.0 } });
        commandBuffer.push_back({ BEGIN_POINTS, { } });
        commandBuffer.push_back({ VERTEX_2D, { point->x, point->y } });
        commandBuffer.push_back({ END_POINTS, { } });
    }



    return commandBuffer;
}

std::vector<uRenderCommand> DrawRectangles(std::vector<Rectangle*> rectangles) {
    std::vector<uRenderCommand> commandBuffer;
    return commandBuffer;
}

std::vector<uRenderCommand> DrawImages(std::vector<Image*> images) {
    std::vector<uRenderCommand> commandBuffer;
    return commandBuffer;
}


