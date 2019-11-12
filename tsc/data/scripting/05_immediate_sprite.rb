# -*- coding: utf-8 -*-

module Std

  ##
  # Class: Std::ImmediateSprite
  #
  # More handy class than the core Sprite class. Instances of this class
  # do not need to have L<#show> called on them to be displayed. As a downside,
  # you have to specify all position-relevant parameters directly on
  # instanciation with L<::new>.
  class ImmediateSprite < Sprite

    ##
    # Method: Std::ImmediateSprite::new
    #
    #   new(path, opts) → an_immediate_sprite
    #
    # Quick’n’easy way to bring a static object into the game. Specifying
    # position and massivity, this method also immediately L<#show>s
    # the object in gameplay.
    #
    # =head4 Parameters
    #
    # =over
    #
    # =item [path]
    #
    # The path to the image file, relative to the F<pixmaps/> directory.
    #
    # =item [opts]
    #
    # A hash with the following arguments:
    #
    # =over
    #
    # =item [x] The X start position. Can contain fractions.
    #
    # =item [y] The Y start position. Can contain fractions.
    #
    # =item [massive (:massive)]
    #
    # The sprite’s massivity type. One of :massive, :passive,
    # :halfmassive, :frontpassive, or :climbable. Defaults to :massive
    # if ommited.
    #
    # =back
    #
    # =back
    def initialize(path, opts)
      x       = opts[:x]       || raise(ArgumentError, ":x not given")
      y       = opts[:y]       || raise(ArgumentError, ":y not given")
      massive = opts[:massive] || :massive

      sprite = super(path)
      sprite.start_at(x, y)
      sprite.massive_type = massive
      sprite.show

      sprite
    end

  end

end
