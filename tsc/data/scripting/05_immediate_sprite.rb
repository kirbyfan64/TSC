# -*- coding: utf-8 -*-

module Std

  ##
  # Class: Std::ImmediateSprite
  #
  # More handy class than the core Sprite class. Instances of this class
  # do not need to have #show called on them to be displayed. As a downside,
  # you have to specify all position-relevant parameters directly on
  # instanciation with ::new.
  class ImmediateSprite < Sprite

    ##
    # Method: Std::ImmediateSprite::new
    #
    #   new(path, opts) → an_immediate_sprite
    #
    # Quick’n’easy way to bring a static object into the game. Specifying
    # position and massivity, this method also immediately <tt>show</tt>s
    # the object in gameplay.
    #
    # <h4>Parameters</h4>
    # <dl>
    # <dt>path</dt>
    # <dd>The path to the image file, relative to the +pixmaps+ directory.</dd>
    # <dt>[opts]</dt>
    # <dd>A hash with the following arguments:
    #   <dl>
    #   <dt>x</dt><dd>The X start position. Can contain fractions.</dd>
    #   <dt>y</dt><dd>The Y start position. Can contain fractions.</dd>
    #   <dt>massive (:massive)</dt>
    #   <dd>The sprite’s massivity type. One of :massive, :passive, :halfmassive, :frontpassive, or :climbable.
    #     Defaults to :massive if ommited.</dd>
    #   </dl>
    # </dd>
    # </dl>
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
