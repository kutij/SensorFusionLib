#include "plotter.h"

using namespace cvplot;

Figure::FigurePtr cvplot::Plotter::_getNthFigure(unsigned int n) {
	return figureMap.at(n);
}

View::ViewPtr cvplot::Plotter::_getNthView(unsigned int n) {
	return viewMap.at(n);
}

Series::SeriesPtr cvplot::Plotter::_getNthSeries(unsigned int plotindex, unsigned int seriesindex) {
	return seriesMap.at(SeriesID(plotindex, seriesindex));
}

void cvplot::Plotter::_setNthFigure(unsigned int n, Figure::FigurePtr figure) {
	figureMap[n] = figure;
}

void cvplot::Plotter::_setNthView(unsigned int n, View::ViewPtr view) {
	viewMap[n] = view;
}

void cvplot::Plotter::_setNthSeries(unsigned int plotindex, unsigned int seriesindex, Series::SeriesPtr series) {
	seriesMap[SeriesID(plotindex, seriesindex)] = series;
}

cvplot::Plotter::Plotter(std::string name_, Offset o, Size plotsize) :
	name(name_), W(cvplot::Window::current(name_)),
	numOfPlots(0), plotSize(plotsize), offset(o),
	figureMap(std::map<unsigned int, Figure::FigurePtr>()),
	viewMap(std::map<unsigned int, View::ViewPtr>()),
	seriesMap(std::map<SeriesID, Series::SeriesPtr>()) {
	W.offset(o);
	W.size(Size(plotsize.width, plotsize.height * 0));
}

void cvplot::Plotter::addPlot(std::string plotName, unsigned int numofsignals, std::vector<std::string> names, bool showLegend) {
	int numofcols = (int)( numOfPlots / MAX_NUM_OF_ROWS) +1;
	int numofrows = numOfPlots + 1;
	if (numofcols > 1)
		numofrows = MAX_NUM_OF_ROWS;
	W.resize(Rect(offset.x, offset.y, plotSize.width * numofcols, plotSize.height * numofrows));
	cvplot::View::ViewPtr view = W.view(name + "_" + std::to_string(numOfPlots), plotSize);
	_setNthView(numOfPlots, view);
	view->offset(cvplot::Offset( (numofcols-1)*plotSize.width,
		(numOfPlots % MAX_NUM_OF_ROWS) * plotSize.height));
	view->title(plotName);

	cvplot::Figure::FigurePtr figure = std::make_shared<cvplot::Figure>(view);
	_setNthFigure(numOfPlots, figure);
	figure->origin(false, false);

	for (unsigned int i = 0; i < numofsignals; i++) {
		std::string seriesname;
		if (i < names.size())
			seriesname = names[i];
		else
			seriesname = name + "_" + std::to_string(numOfPlots) + "_" + std::to_string(i);
		_setNthSeries(numOfPlots, i, figure->series(seriesname));
		figure->series(seriesname)->legend(showLegend);
	}

	figure->show(false);
	view->finish();
	view->flush();

	numOfPlots++;
}

void cvplot::Plotter::setLineType(unsigned int plotindex, unsigned int seriesindex, Type type) {
	_getNthSeries(plotindex, seriesindex)->type(type);
}

void cvplot::Plotter::setLineColor(unsigned int plotindex, unsigned int seriesindex, Color color) {
	_getNthSeries(plotindex, seriesindex)->color(color);
}

void cvplot::Plotter::addValue(unsigned int plotindex, unsigned int seriesindex, float t, float value) {
	 _getNthSeries(plotindex, seriesindex)->add1({ t,value });
}

void cvplot::Plotter::updatePlot(unsigned int plotindex) {
	auto view = _getNthView(plotindex);
	auto figure = _getNthFigure(plotindex);
	figure->show(false);
	view->finish();
	view->flush();
}

void cvplot::Plotter::updateWindow() {
	W.dirty();
	W.flush();
}
