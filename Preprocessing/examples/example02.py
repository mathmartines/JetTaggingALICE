"""Plot the average over all the events"""

import numpy as np
from Preprocessing.Image import Image, SingleImageBuilder, AverageImageBuilder
import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap

if __name__ == "__main__":
    # reading the csv file
    hepmc_data = np.genfromtxt("/sampa/archive/caducka/jetsml/ccbar_prod_40_60.csv", delimiter=",")

    # set up the configurations of the image
    image = Image(
        eta_min=-5, eta_max=5, phi_min=-np.pi, phi_max=np.pi, n_bins_eta=15, n_bins_phi=15, num_colors=2,
        image_builder=AverageImageBuilder(image_builder=SingleImageBuilder())
    )

    # creating the image corresponding to the average
    image_grid = image.create_image(hepmc_data)

    # Create a custom colormap that transitions from white to blue to red
    colors = [(1, 1, 1), (0, 0, 1), (1, 0, 0)]  # White, Blue, Red
    n_bins = 100  # Number of bins for interpolation
    custom_cmap = LinearSegmentedColormap.from_list('white_blue_red', colors, N=n_bins)
    plt.imshow(image_grid[:, :, 0], cmap=custom_cmap, extent=(-np.pi, np.pi, -4, 4), vmin=0, vmax=1)
    plt.colorbar(label='pT')
    plt.xlabel(r'$\phi^{rel}$')
    plt.ylabel(r'$\eta^{rel}$')
    plt.title(r'Average Pt - c cbar production')
    plt.savefig('avg_pt_ccbar.png', bbox_inches="tight", dpi=300)

    plt.imshow(image_grid[:, :, 1], cmap="bwr", extent=(-np.pi, np.pi, -4, 4), vmin=-0.1, vmax=0.1)
    plt.colorbar(label='Charge')
    plt.xlabel(r'$\phi^{rel}$')
    plt.ylabel(r'$\eta^{rel}$')
    plt.title(r'Average Charge - c cbar production')
    plt.savefig('avg_charge_ccbar.png', bbox_inches="tight", dpi=300)