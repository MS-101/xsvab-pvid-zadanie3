# Počítačové videnie - úloha 3

**Autor: Martin Šváb**

## VOC12 segmentácia

### Vstupné obrázky

Na porovnanie rôznych metód segmentácie sme si vybrali 3 obrázky. V tomto súbore však budeme zobrazovať výstupy len z jedného z nich. Pre kompletné výsledky experimentovania si pozrite obrázky v priečinku output. 

![](input/VOC12/images/2007_000033.jpg)

### Pravda

Z každého vstupného obrázku sa snažíme segmentovať červený objekt. Teda upravíme si náš label, tak že z neho vyrobíme masku, kde biely pixel reprezentuje objekt a čierny pixel reprezentuje pozadie. Na nasledujúcom obrázku vidíte hore vstupný obrázok a jeho label. Dole vidíte ako sme tento label upravili na masku a po aplikovaní tejto masky na vstupnom obrázku vidíme vybraný objekt. Tieto masky potom využíme pri všetkých našich experimentov.

![](output/VOC12/truth/2007_000033.jpg)

### Grabcut

Pre tento experiment sme si pre naše obrázky manuálne vytvorili masku, kde biela farba reprezentuje objekt (foreground), čierna farba reprezentuje pozadie (background) a šedá farba reprezentuje neznámu hodnotu. Túto masku potom upravujeme v iteráciach grabcut algoritmu.

![](input/VOC12/masks/2007_000033.jpg)

Vo výstupnom obrázku môžete hore vidieť vstupný obrázok a jeho ručne vytvorenú masku.
V druhom riadku vidíte masku vytvorenú po 10 iteráciach grabcut algoritmu na pôvodnej maske a segmentovaný obrázok vytvorený aplikáciou tejto vypočítanej masky. Dole vidíte pravdivú masku a segmentovaný obrázok. Tento experiment bol úspešný a dosahoval vysoké dice skóre.

	void grabcut(std::string name, cv::Mat input, cv::Mat mask, cv::Mat truthMask, cv::Mat truth)
	{
		cv::Mat maskGrabcut;
		toGrabcutMask(mask, maskGrabcut);

		cv::Mat foreground, background;
		cv::grabCut(input, maskGrabcut, cv::Rect(), background, foreground, 10, cv::GC_INIT_WITH_MASK);

		cv::Mat maskAfter;
		toDisplayMask(maskGrabcut, maskAfter);

		cv::Mat output;
		applyMask(input, maskAfter, output);

		outputGrabcut(name, input, mask, maskAfter, output, truthMask, truth);
	}

![](output/VOC12/grabcut/2007_000033.jpg)

Vypočítané dice skóre: 0.809587

### Superpixely

Vo výstupnom obrázku môžete hore vidieť vstupný obrázok a následne jeho rozmazanú verziu po aplikácii gausovského filtra. Obrázok sme rozmazali, lebo obsahoval príliš veľa detailov. Obrázok sme museli konvertovať do LAB farebného priestoru pre správne fungovanie algoritmu. V druhom riadku vidíte masku vytvorenú po 50 iteráciach SLIC algoritmu a pôvodný obrázok spojený s touto zafarbenou maskou. Dole vidíte pravdivú masku a segmentovaný obrázok.

	void superpixels(std::string name, cv::Mat input, cv::Mat truthMask, cv::Mat truth)
	{
		cv::Mat blur;
		cv::GaussianBlur(input, blur, cv::Size(5, 5), 0);

		cv::Mat converted;
		cvtColor(blur, converted, cv::COLOR_BGR2Lab);
		
		auto slic = cv::ximgproc::createSuperpixelSLIC(converted, cv::ximgproc::SLIC, 25, 10.0f);
		slic->iterate(50);

		cv::Mat mask;
		slic->getLabelContourMask(mask, false);

		cv::Mat output = input.clone();
		output.setTo(cv::Scalar(0, 0, 255), mask);

		outputSuperpixels(name, input, blur, mask, output, truthMask, truth);
	}

![](output/VOC12/superpixels/2007_000033.jpg)

Pri tomto algoritme sme nepočítali dice skóre.

### Floodfill

Vo výstupnom obrázku môžete hore vidieť vstupný obrázok. V druhom riadku vidíte masku vytvorenú po aplikácii floodfill algoritmu a obrázok získaný aplikáciou tejto masky. Na binarizáciu obrázku sme použili OTSU thresholding, seed point je nastavený na pozíciu [0, 0]. Dole vidíte pravdivú masku a segmentovaný obrázok.

	void floodfill(std::string name, cv::Mat input, cv::Mat truthMask, cv::Mat truth)
	{
		cv::Mat gray;
		cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

		cv::Mat threshold;
		cv::threshold(gray, threshold, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

		cv::Mat mask = threshold.clone();
		cv::Point seedPoint(0, 0);
		cv::floodFill(mask, seedPoint, cv::Scalar(255));
		cv::bitwise_not(mask, mask);

		cv::Mat output;
		applyMask(input, mask, output);

		outputFloodfill(name, input, mask, output, truthMask, truth);
	}

![](output/VOC12/floodfill/2007_000033.jpg)

Vypočítané dice skóre: 0.0423924

### Threshold

Vo výstupnom obrázku môžete hore vidieť vstupný obrázok. V druhom riadku vidíte masku vytvorenú po aplikácii OTSU thresholding algoritmu a obrázok získaný aplikáciou tejto masky. Dole vidíte pravdivú masku a segmentovaný obrázok.

	void threshold(std::string name, cv::Mat input, cv::Mat truthMask, cv::Mat truth)
	{
		cv::Mat gray;
		cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

		cv::Mat mask;
		cv::threshold(gray, mask, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

		cv::Mat output;
		applyMask(input, mask, output);

		outputThreshold(name, input, mask, output, truthMask, truth);
	}

![](output/VOC12/threshold/2007_000033.jpg)

Vypočítané dice skóre: 0.315691

## Motion tracking

TBA
