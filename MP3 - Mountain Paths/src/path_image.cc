#include "path_image.hpp"

PathImage::PathImage(const GrayscaleImage &image, const ElevationDataset &dataset) {
    width_ = image.Width();
    height_ = image.Height();
    path_image_ = image.GetImage();
    paths_ = std::vector<Path>(height_, Path(width_, 0));
    InitializePaths(dataset);
    DrawPaths();
}
size_t PathImage::Width() const { return width_; }
size_t PathImage::Height() const { return height_; }
unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }
const std::vector<Path>& PathImage::Paths() const { return paths_; }
const std::vector<std::vector<Color> >& PathImage::GetPathImage() const { return path_image_; }
void PathImage::ToPpm(const std::string& name) const {
    std::ofstream ofs {name};
    ofs << "P3" << "\n";
    ofs << width_ << " " << height_ << "\n";
    ofs << kMaxColorValue << "\n";
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            Color c = path_image_[i][j];
            ofs << c.Red() << " " << c.Green() << " " << c.Blue() << " ";
        }
        ofs << "\n";
    }
}
void PathImage::InitializePaths(const ElevationDataset &dataset) {
    paths_ = std::vector<Path>(height_, Path(width_, 0));
    for (size_t i = 0; i < height_; i++) {
        Path p(width_, i);
        p.SetLoc(0, i);
        size_t row = i;
        for (size_t j = 1; j < width_; j++) {
            UpdatePath(p, dataset, row, j);
        }
        paths_[i] = p;
    }
}
void PathImage::UpdatePath(Path& p, const ElevationDataset& dataset, size_t& row, size_t j) const {
    int prev = dataset.DatumAt(row, j - 1);
    int forward = std::abs(dataset.DatumAt(row, j) - prev);
    if (row == 0) {
        int south_east  = std::abs(dataset.DatumAt(row + 1, j) - prev);
        if (south_east < forward) {
            ++row;
            p.IncEleChange(south_east);
        } else {
            p.IncEleChange(forward);
        } 
    } else if (row == height_ - 1) {
        int north_east  = std::abs(dataset.DatumAt(row - 1, j) - prev);
        if (north_east < forward) {
            --row;
            p.IncEleChange(north_east);
        } else {
            p.IncEleChange(forward);
        }
    } else {
        int south_east  = std::abs(dataset.DatumAt(row + 1, j) - prev);
        int north_east  = std::abs(dataset.DatumAt(row - 1, j) - prev);
        if (south_east < forward && south_east <= north_east) {
            ++row;
            p.IncEleChange(south_east);
        } else if (north_east < forward && north_east < south_east) {
            --row;
            p.IncEleChange(north_east);
        } else {
            p.IncEleChange(forward);
        } 
    }
    p.SetLoc(j, row);
}
void PathImage::DrawPaths() {
    Color c (kRedR,kRedG,kRedB);
    size_t min = 0;
    unsigned int min_change = paths_[0].EleChange();
    for (size_t i = 0; i < height_; i++) {
        if (paths_[i].EleChange() < min_change) {
            min_change = paths_[i].EleChange();
            min = i;
        }
        for (size_t j = 0; j < width_; j++) {
            size_t num = paths_[i].GetPath()[j];
            path_image_[num][j] = c;
        }
    }
    Color green(kGreenR,kGreenG,kGreenB);
    for (size_t i = 0; i < width_; i++) {
        size_t num = paths_[min].GetPath()[i];
        path_image_.at(num).at(i) = green;
    }
}