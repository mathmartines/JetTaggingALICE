"""Constructing the image for a single event"""

import numpy as np
from Preprocessing.Image import Image, SingleImageBuilder
import matplotlib.pyplot as plt


if __name__ == "__main__":
    # reading the csv file
    hepmc_data = np.genfromtxt("/Users/martines/Desktop/Physics/pythia8312/examples/ccbar_production_pt_10_35_GeV.csv", delimiter=",")

    # set up the configurations of the image
    image = Image(
        eta_min=-10, eta_max=10, phi_min=-10, phi_max=10, n_bins_eta=20, n_bins_phi=20, num_colors=2,
        image_builder=SingleImageBuilder()
    )

    # creating the image for a single event
    image_grid = image.create_image(hepmc_data[0])

    print(image_grid)
