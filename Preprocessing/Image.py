from __future__ import annotations
import numpy as np
from abc import ABC, abstractmethod
from collections import UserDict


class Image:
    """
    Constructs a grid that represents the event or jet as an image.
    The number of colors in each pixel, as well as the pixels dimensions and limits, are given by the user.
    """

    def __init__(self, eta_min: float, eta_max: float, phi_min: float, phi_max: float,
                 n_bins_eta: int, n_bins_phi: int, num_colors: int, image_builder: ImageBuilder):
        # minimum and maximum allowed value for the eta axis
        self._eta_min, self._eta_max = eta_min, eta_max
        # minimum and maximum allowed value for the phi axis
        self._phi_min, self._phi_max = phi_min, phi_max
        # number of bins in the eta and phi axis
        self._nbins_eta, self._nbins_phi = n_bins_eta, n_bins_phi
        # evaluating the bin sizes
        self._eta_bin_size = (self._eta_max - self._eta_min) / self._nbins_eta
        self._phi_bin_size = (self._phi_max - self._phi_min) / self._nbins_phi
        # number of color channels in the image
        self._num_colors = num_colors
        # create the grid to store the color intensity in each pixel
        self._image = np.zeros(shape=(n_bins_eta, n_bins_phi, num_colors))
        # image builder responsible to construct the image
        self._image_builder = image_builder

    def create_image(self, image_data):
        """Constructs the image out of the image info"""
        # reset the grid
        self._image[:] = 0
        # delegates the task to the ImageBuilder instance, that knows how to handle the information
        self._image_builder.build_image(image_data=image_data, image=self)
        # returns a copy of the constructed image
        return np.copy(self._image)

    def update_image(self, eta: float, phi: float, colors: np.array):
        """Updates the bin containning the values of eta and phi with the respective color intensitities"""
        # the eta bin edge starts from the maximum value and goes to the minimum
        # example: [2.0, 1.9, ..., -1.9, -2.0]
        eta_bin = self._nbins_eta - int((eta - self._eta_min) / self._eta_bin_size) - 1
        # the phi bin edge starts from the minimum value
        # example: [-2.0, -1.9, ..., 1.9, 2.0]
        phi_bin = int((phi - self._phi_min) / self._phi_bin_size)
        # updating the image only if it's within the allowed range
        if 0 <= eta_bin < self._nbins_eta and 0 <= phi_bin < self._nbins_phi:
            self._image[eta_bin][phi_bin] += colors

    def rescale_image(self, factor):
        """rescale all the entries by factor."""
        self._image *= factor


class ImageBuilder(ABC):
    """Construct the image"""
    @abstractmethod
    def build_image(self, image_data, image: Image):
        """reconstructs the image from the available data"""
        pass


class ChargesMap(UserDict):
    """
    Map to store the value of the charges given the PID of the particle
    Only needs to store the PID for the particle. The charge of the antiparticle is -Charge where
    Charge is the electric charge of the respective particle.
    """
    def __getitem__(self, pid):
        if abs(pid) in self.data:
            return self.data[pid] if pid in self.data else -self.data[-pid]
        return 0

    def __setitem__(self, pid, charge):
        if abs(pid) not in self.data:
            self.data[pid] = charge


class SingleImageBuilder(ImageBuilder):
    """Builder for a single image using all the available info"""
    # PID of the particles and their respective charge
    pid_charges_map = ChargesMap({211: 1, 11: -1, 13: -1, 321: 1, 2212: 1})

    def build_image(self, image_data, image: Image):
        # info about the particle with the greates pT
        # assumes that the particles are sorted by pT, from the biggest to the lowest (this is the case for the files)
        pt_ref, eta_ref, phi_ref = image_data[2: 5]
        # updates the image with each particle info that belongs to the event
        for index_const in range(2, len(image_data), 4):
            # particle information
            pt, eta, phi, pid = image_data[index_const: index_const + 4]
            # array with the color intensities
            color_intensities = np.array([pt/pt_ref, self.pid_charges_map[pid]])
            # updating the image
            image.update_image(eta=eta - eta_ref, phi=self.delta_phi(phi, phi_ref), colors=color_intensities)

    @staticmethod
    def delta_phi(phi1, phi2):
        """Evaluates the difference among the azimuthal angles"""
        dphi = phi1 - phi2
        while dphi >= np.pi:
            dphi -= 2 * np.pi
        while dphi < -np.pi:
            dphi += 2 * np.pi
        return dphi


class AverageImageBuilder(ImageBuilder):
    """Takes the average over the images"""

    def __init__(self, image_builder: ImageBuilder):
        # stores the builder that must be used over the instances
        self._image_builder = image_builder

    def build_image(self, image_data, image: Image):
        # total number of instances
        number_evts = image_data.shape[0]
        # iterates over all the events
        for event in image_data:
            self._image_builder.build_image(event, image)
        # normalize the entries by the total number of instances
        image.rescale_image(1/number_evts)
