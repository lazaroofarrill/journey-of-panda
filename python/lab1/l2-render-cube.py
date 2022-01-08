from direct.showbase.ShowBase import ShowBase


class MyApp(ShowBase):

    def __init__(self):
        ShowBase.__init__(self)
        # # Load the environment model.
        # first_model = self.loader.load_model('cube.gltf')
        # first_model.reparent_to(self.render)
        # first_model.set_pos(0, 0, 0.5)
        # first_model.set_scale(1)
        #
        # self.first_model = first_model
        # self.first_model.reparentTo(self.render)


app = MyApp()
app.run()
